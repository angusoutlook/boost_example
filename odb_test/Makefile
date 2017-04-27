driver:driver.o person-odb.o
	g++ -o driver driver.o person-odb.o -lodb -lodb-mysql
person-odb.o: person-odb.cxx
	g++ -c person-odb.cxx
driver.o:driver.cxx
	g++ -c driver.cxx
clean:
	rm -rf *.o
	rm -rf person-odb.*
	odb -d mysql --generate-query --generate-schema person.hxx
