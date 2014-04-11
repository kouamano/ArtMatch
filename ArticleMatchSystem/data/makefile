all: arts.idx

arts.idx: *.title
	./create_dbform_from_Title.sh target.title -T > arts.idx

install:
	cp arts.idx /usr/local/var/arts/
