GALOIS_DIR = GaloisFieldArithmetic

encoder: encoder.o  $(GALOIS_DIR)/GaloisField.o
	g++ -o encoder.out encoder.o $(GALOIS_DIR)/GaloisField.o

$(GALOIS_DIR)/GaloisField.o: $(GALOIS_DIR)/GaloisField.cpp
	g++ -c $(GALOIS_DIR)/GaloisField.cpp

encoder.o: encoder.cpp
	g++ -c encoder.cpp

clean:	
	rm -f *.o *.out 
