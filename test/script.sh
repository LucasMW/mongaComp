
FILES="test/simple/*.monga"
for f in $FILES
do
	name=$(echo $f | cut -f 1 -d '.')
	echo "Testing $name"
	cat $f | ./comp > $f.output
	if [ ! -f $name.answer ]; then
    	echo "no answer associated"
    	continue
	fi
	result=$(cmp $f.output $name.answer)
	if($result) then
		echo "OK"
		rm $f.output
	else
		echo "Wrong"
		diff $f.output $name.answer
	fi
done
#cat test/file1.monga | ./comp | grep "tokens"