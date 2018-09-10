package main

import (
	"fmt"
	"os"
	"github.com/sirupsen/logrus"
	"github.com/sergi/go-diff/diffmatchpatch"
)

const (
	text1 = "Lorem ipsum dolor."
	text2 = "Lorem dolor sit amet."
)


func main() {
	logrus.SetOutput(os.Stdout)
	logrus.SetLevel(logrus.DebugLevel)
	logrus.Debugf("debug message")
	var obj *int = nil
	logrus.Debugf(fmt.Sprintf("%v", obj))

	text3 := "old resource pool is &{{HAWQResourcePool rm.pivotaldata.io/v1alpha1} {hawq-resource-pool  default /apis/rm.pivotaldata.io/v1alpha1/namespaces/default/hawqresourcepools/hawq-resource-pool 927f9bc4-a9a0-11e8-8a07-42010a8000eb 9043395 1 2018-08-27 02:26:15 +0000 UTC <nil> <nil> map[resourcepool:hawq-resource-pool] map[] [] nil [] } {default &LabelSelector{MatchLabels:map[string]string{resourcepool: hawq-resource-pool,},MatchExpressions:[],} {4000 6144 3072} hawqbeijing/hawq_for_rm [{group1 2 {250 1024 1024}} {group2 2 {250 512 512}}] FastAllocatePolicy} {ready {1000 3072 3072} [{group1 2 0} {group2 2 0}]}}. \n"
	text4 := "new resource pool is &{{ } {hawq-resource-pool  default /apis/rm.pivotaldata.io/v1alpha1/namespaces/default/hawqresourcepools/hawq-resource-pool 927f9bc4-a9a0-11e8-8a07-42010a8000eb 9060889 1 2018-08-27 02:26:15 +0000 UTC <nil> <nil> map[resourcepool:hawq-resource-pool] map[] [] nil [] } {default &LabelSelector{MatchLabels:map[string]string{resourcepool: hawq-resource-pool,},MatchExpressions:[],} {4000 6144 3072} hawqbeijing/hawq_for_rm [{group1 2 {250 1024 1024}} {group2 2 {250 512 512}}] FastAllocatePolicy} {creating {0 0 0} [{group1 2 0} {group2 2 0} {group1 0 0} {group2 0 0}]}},\n"

	dmp := diffmatchpatch.New()

	diffs := dmp.DiffMain(text1, text2, false)
	fmt.Println(dmp.DiffPrettyText(diffs))
	//fmt.Println(dmp.DiffText1(diffs))
	diffs = dmp.DiffMain(text3, text4, false)
	fmt.Printf("diff %s", dmp.DiffPrettyText(diffs))
	logrus.Debugf("logrus diff %s", dmp.DiffPrettyText(diffs))
}
