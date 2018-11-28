GALOIS_DIR=GaloisFieldArithmetic

test: test.o   $(GALOIS_DIR)/GaloisField.o $(GALOIS_DIR)/GaloisFieldElement.o $(GALOIS_DIR)/GaloisFieldPolynomial.o
	g++ -o test.out test.o $(GALOIS_DIR)/GaloisField.o $(GALOIS_DIR)/GaloisFieldElement.o $(GALOIS_DIR)/GaloisFieldPolynomial.o

encoder: encoder.o  $(GALOIS_DIR)/GaloisField.o $(GALOIS_DIR)/GaloisFieldElement.o $(GALOIS_DIR)/GaloisFieldPolynomial.o
	g++ -o encoder.out encoder.o $(GALOIS_DIR)/GaloisField.o $(GALOIS_DIR)/GaloisFieldElement.o $(GALOIS_DIR)/GaloisFieldPolynomial.o

$(GALOIS_DIR)/GaloisField.o: $(GALOIS_DIR)/GaloisField.cpp
	g++ -c $(GALOIS_DIR)/GaloisField.cpp -o $(GALOIS_DIR)/GaloisField.o

$(GALOIS_DIR)/GaloisFieldElement.o: $(GALOIS_DIR)/GaloisFieldElement.cpp
	g++ -c $(GALOIS_DIR)/GaloisFieldElement.cpp -o $(GALOIS_DIR)/GaloisFieldElement.o

$(GALOIS_DIR)/GaloisFieldPolynomial.o: $(GALOIS_DIR)/GaloisFieldPolynomial.cpp
	g++ -c $(GALOIS_DIR)/GaloisFieldPolynomial.cpp -o $(GALOIS_DIR)/GaloisFieldPolynomial.o

test.o: test.cpp
	g++ -c test.cpp

encoder.o: encoder.cpp
	g++ -c encoder.cpp

clean:	
	rm -f *.o *.out 
