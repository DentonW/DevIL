run: DocBuilder.class
	java DocBuilder DevilDoc.xml

java: DocBuilder.java
	javac DocBuilder.java

DocBuilder.class: DocBuilder.java
	javac DocBuilder.java

dist:
	mkdir Docs
	mv index.html Constants IL ILU ILUT images Docs
	tar jcf Docs.tar.bz2 Docs
	rm -r Docs

clean:
	rm *.class
