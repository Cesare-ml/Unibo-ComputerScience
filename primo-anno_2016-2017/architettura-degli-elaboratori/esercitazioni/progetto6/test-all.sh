cd src
make
cd ..

for fn in $(ls | grep .vm); do
	./src/VMtranslator "$fn"
done

echo ""
for fn in $(ls | grep .tst); do
	echo $fn
	CPUEmulator.sh "$fn"
	echo ""
done
