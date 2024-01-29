JFLAGS = -g 
JC = javac
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	Q1.java \
	Q2.java \
	CustomSkipList.java \
	MergeSort.java \
	BinaryTree.java \
	BinarySearchTree.java \
	Main.java 

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class
