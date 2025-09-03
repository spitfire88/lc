#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <errno.h>

static long
perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                int cpu, int group_fd, unsigned long flags)
{
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

int main()
{
    struct perf_event_attr pe;
    memset(&pe, 0, sizeof(struct perf_event_attr));
    pe.type = PERF_TYPE_SOFTWARE;
    pe.size = sizeof(struct perf_event_attr);
    pe.config = PERF_COUNT_SW_CPU_CLOCK;
    pe.sample_period = 1000000; // Sample every 1ms of CPU time
    pe.sample_type = PERF_SAMPLE_IP | PERF_SAMPLE_CALLCHAIN;
    pe.disabled = 1;
    pe.exclude_kernel = 0; // Include kernel
    pe.exclude_user = 0;   // Include user
    pe.precise_ip = 2;     // Try to get precise IP

    int fd = perf_event_open(&pe, 0, -1, -1, 0);
    if (fd == -1) {
        perror("perf_event_open");
        exit(EXIT_FAILURE);
    }

    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);

    FILE *out = fopen("perf_stack_dump.txt", "w");
    if (!out) {
        perror("fopen");
        close(fd);
        exit(EXIT_FAILURE);
    }


    // mmap ring buffer setup
    size_t page_size = sysconf(_SC_PAGESIZE);
    size_t mmap_pages = 8; // 8 pages for ring buffer
    size_t mmap_len = (mmap_pages + 1) * page_size;
    void *mmap_buf = mmap(NULL, mmap_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mmap_buf == MAP_FAILED) {
        perror("mmap");
        fclose(out);
        close(fd);
        exit(EXIT_FAILURE);
    }

    struct perf_event_mmap_page *metadata = (struct perf_event_mmap_page *)mmap_buf;
    char *data = (char *)mmap_buf + page_size;

    for (int i = 0; i < 10; ++i) {
        // Busy loop to generate CPU activity
        volatile unsigned long busy = 0;
        for (unsigned long k = 0; k < 10000000; ++k) {
            busy += k;
        }
        __sync_synchronize();
        uint64_t data_head = metadata->data_head;
        __sync_synchronize();
        uint64_t data_tail = metadata->data_tail;
        size_t buffer_size = mmap_pages * page_size;

        while (data_tail < data_head) {
            size_t offset = data_tail % buffer_size;
            struct {
                struct perf_event_header header;
                char data[0];
            } *event = (void *)(data + offset);

            if (event->header.type == PERF_RECORD_SAMPLE) {
                size_t sample_offset = sizeof(struct perf_event_header);
                uint64_t ip = *(uint64_t *)(event->data + 0);
                sample_offset += sizeof(uint64_t);
                uint64_t nr = *(uint64_t *)(event->data + sizeof(uint64_t));
                sample_offset += sizeof(uint64_t);
                fprintf(out, "Sample: IP: 0x%llx, Callchain depth: %llu\n", (unsigned long long)ip, (unsigned long long)nr);
                for (uint64_t j = 0; j < nr; ++j) {
                    uint64_t addr = *(uint64_t *)(event->data + sample_offset);
                    sample_offset += sizeof(uint64_t);
                    fprintf(out, "  0x%llx\n", (unsigned long long)addr);
                }
                fprintf(out, "-----------------------------\n");
            }
            data_tail += event->header.size;
        }
        metadata->data_tail = data_head;
        usleep(500000); // 0.5s
    }

    munmap(mmap_buf, mmap_len);

    fclose(out);
    close(fd);
    return 0;
}