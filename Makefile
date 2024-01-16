add:
	git add *.py

commit:
	git commit -m "update file python"

push:
	git push origin dev/nancy

all:
	add
	commit
	push