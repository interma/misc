import sys


#default params
def foo(word, who = "interma"):
    print "%s say: %s" % (who, word)


if __name__ == "__main__":
    foo("hello", "I")
    foo("hello")
    sys.exit(0)
