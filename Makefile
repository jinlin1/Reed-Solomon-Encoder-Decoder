GALOIS_DIR=GaloisFieldArithmetic

main: main.o Encoder.o Decoder.o Parse.o Package.o $(GALOIS_DIR)/GaloisField.o $(GALOIS_DIR)/GaloisFieldElement.o $(GALOIS_DIR)/GaloisFieldPolynomial.o 
	g++ -o main main.o Encoder.o Decoder.o Parse.o Package.o $(GALOIS_DIR)/GaloisField.o $(GALOIS_DIR)/GaloisFieldElement.o $(GALOIS_DIR)/GaloisFieldPolynomial.o `pkg-config gtkmm-3.0 --cflags --libs`

main.o: main.cpp
	g++ -c main.cpp `pkg-config gtkmm-3.0 --cflags --libs` 

test: test.o  $(GALOIS_DIR)/GaloisField.o $(GALOIS_DIR)/GaloisFieldElement.o $(GALOIS_DIR)/GaloisFieldPolynomial.o
	g++ -o test.out test.o $(GALOIS_DIR)/GaloisField.o $(GALOIS_DIR)/GaloisFieldElement.o $(GALOIS_DIR)/GaloisFieldPolynomial.o

$(GALOIS_DIR)/GaloisField.o: $(GALOIS_DIR)/GaloisField.cpp
	g++ -c $(GALOIS_DIR)/GaloisField.cpp -o $(GALOIS_DIR)/GaloisField.o

$(GALOIS_DIR)/GaloisFieldElement.o: $(GALOIS_DIR)/GaloisFieldElement.cpp
	g++ -c $(GALOIS_DIR)/GaloisFieldElement.cpp -o $(GALOIS_DIR)/GaloisFieldElement.o

$(GALOIS_DIR)/GaloisFieldPolynomial.o: $(GALOIS_DIR)/GaloisFieldPolynomial.cpp
	g++ -c $(GALOIS_DIR)/GaloisFieldPolynomial.cpp -o $(GALOIS_DIR)/GaloisFieldPolynomial.o

Package.o: Package.cpp
	g++ -c Package.cpp

Parse.o: Parse.cpp
	g++ -c Parse.cpp

Encoder.o: Encoder.cpp
	g++ -c Encoder.cpp 

Decoder.o: Decoder.cpp
	g++ -c Decoder.cpp

test.o: test.cpp
	g++ -c test.cpp

clean:	
	rm -f *.o *.out 
