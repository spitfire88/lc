package sysutils

import (
	"fmt"
)

// Service provides a unified interface to request system utility wrappers.
type Service struct{}

func NewService() *Service {
	return &Service{}
}

func (s *Service) Strace(args ...string) (string, error) {
	return RunStrace(args...)
}

func (s *Service) Perf(args ...string) (string, error) {
	return RunPerf(args...)
}

func (s *Service) Dmesg(args ...string) (string, error) {
	return RunDmesg(args...)
}

func (s *Service) Lsof(args ...string) (string, error) {
	return RunLsof(args...)
}

func (s *Service) Sysctl(args ...string) (string, error) {
	return RunSysctl(args...)
}

// Example usage
func Example() {
	service := NewService()
	if out, err := service.Dmesg(); err == nil {
		fmt.Println(out)
	}
}
