package main

import (
	"fmt"
	. "github.com/onsi/ginkgo"
	logf "sigs.k8s.io/controller-runtime/pkg/runtime/log"
)

func main() {
	logf.SetLogger(logf.ZapLoggerTo(GinkgoWriter, true))
	var log = logf.Log.WithName("main")
	log.V(1).Info("Registering Components.")

	fmt.Print("here")
}
