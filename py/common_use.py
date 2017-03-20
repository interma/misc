import sys


#default params
def foo(word, who = "interma", times = 1):
    for i in range(0,times):
        print "%s say: %s" % (who, word)


if __name__ == "__main__":
    foo("hello", times = 2, who = "I")
    foo("hello")
    sys.exit(0)
