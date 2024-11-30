package main

import (
	"fmt"
	. "github.com/onsi/ginkgo"
	logf "sigs.k8s.io/controller-runtime/pkg/runtime/log"
)

func main() {
	logf.SetLogger(logf.ZapLoggerTo(GinkgoWriter, true))
	logf.SetLogger(logf.ZapLogger(true))
	var log = logf.Log.WithName("main")
	log.V(1).Info("Registering Components.")
	log.Error(fmt.Errorf("i am a inner error"), "outer error")
	log.V(1).Info("xxx info")

	fmt.Print("here")
}
