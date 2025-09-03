import (
	// ...existing imports...
	"io/ioutil"
	"errors"
)
// ParseRamdumpCallstack parses a ramdump file and extracts the callstack at panic.
// It looks for lines containing 'panic' and collects the following lines as the callstack.
func ParseRamdumpCallstack(filepath string) ([]string, error) {
	data, err := ioutil.ReadFile(filepath)
	if err != nil {
		return nil, err
	}
	lines := strings.Split(string(data), "\n")
	var callstack []string
	found := false
	for i, line := range lines {
		if strings.Contains(line, "panic") {
			found = true
			// Collect up to 20 lines after panic (or until empty line)
			for j := i; j < len(lines) && j < i+20; j++ {
				l := strings.TrimSpace(lines[j])
				if l == "" && j > i {
					break
				}
				callstack = append(callstack, lines[j])
			}
			break
		}
	}
	if !found {
		return nil, errors.New("panic not found in ramdump")
	}
	return callstack, nil
}
package sysutils

import (
	"os/exec"
	"strings"
	"regexp"
)

// RunStrace runs strace on the given command and returns its output.
func RunStrace(args ...string) (string, error) {
	cmd := exec.Command("strace", args...)
	out, err := cmd.CombinedOutput()
	return string(out), err
}

// ParseStraceOutput extracts a summary line from strace output (e.g., syscall summary).
func ParseStraceOutput(output string) string {
	lines := strings.Split(output, "\n")
	for _, line := range lines {
		if strings.Contains(line, "--- ") {
			return line
		}
	}
	if len(lines) > 0 {
		return lines[len(lines)-1]
	}
	return ""
}

// RunPerf runs perf with the given arguments and returns its output.
func RunPerf(args ...string) (string, error) {
	cmd := exec.Command("perf", args...)
	out, err := cmd.CombinedOutput()
	return string(out), err
}

// ParsePerfOutput extracts the first line with 'samples' or a summary from perf output.
func ParsePerfOutput(output string) string {
	lines := strings.Split(output, "\n")
	for _, line := range lines {
		if strings.Contains(line, "samples") || strings.Contains(line, "Event count") {
			return line
		}
	}
	if len(lines) > 0 {
		return lines[0]
	}
	return ""
}

// RunDmesg runs dmesg and returns its output.
func RunDmesg(args ...string) (string, error) {
	cmd := exec.Command("dmesg", args...)
	out, err := cmd.CombinedOutput()
	return string(out), err
}

// ParseDmesgOutput extracts the last kernel message from dmesg output.
func ParseDmesgOutput(output string) string {
	lines := strings.Split(strings.TrimSpace(output), "\n")
	if len(lines) > 0 {
		return lines[len(lines)-1]
	}
	return ""
}

// RunLsof runs lsof with the given arguments and returns its output.
func RunLsof(args ...string) (string, error) {
	cmd := exec.Command("lsof", args...)
	out, err := cmd.CombinedOutput()
	return string(out), err
}

// ParseLsofOutput extracts the first open file entry from lsof output (skipping header).
func ParseLsofOutput(output string) string {
	lines := strings.Split(output, "\n")
	if len(lines) > 1 {
		return lines[1]
	}
	return ""
}

// RunSysctl runs sysctl with the given arguments and returns its output.
func RunSysctl(args ...string) (string, error) {
	cmd := exec.Command("sysctl", args...)
	out, err := cmd.CombinedOutput()
	return string(out), err
}

// ParseSysctlOutput extracts the value from a sysctl output line (e.g., 'key = value').
func ParseSysctlOutput(output string) string {
	re := regexp.MustCompile(`^\s*[^=]+\s*=\s*(.*)$`)
	lines := strings.Split(output, "\n")
	for _, line := range lines {
		if m := re.FindStringSubmatch(line); m != nil {
			return m[1]
		}
	}
	return strings.TrimSpace(output)
}
