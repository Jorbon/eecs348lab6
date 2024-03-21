#include <iostream>
#include <fstream>
#include <vector>

#define Matrix std::vector<std::vector<int>>

#define INPUT_FILE_NAME "matrix_input.txt"


int get_int_characters(int n) {
	int count = 1;
	if (n < 0) count += 1;
	while ((n /= 10) != 0) count += 1;
	return count;
}

void matrix_print(const Matrix &m) {
	// First find the widest that a number in the matrix will be
	int max_width = 0;
	int width = 0;
	for (int i = 0; i < m.size(); i++) {
		for (int j = 0; j < m.size(); j++) {
			width = get_int_characters(m[i][j]);
			if (width > max_width) max_width = width;
		}
	}
	
	for (int i = 0; i < m.size(); i++) {
		for (int j = 0; j < m.size(); j++) {
			// Pad all values in the matrix to be the same width
			printf("%*d ", max_width, m[i][j]);
		}
		printf("\n");
	}
}

Matrix matrix_add(const Matrix &a, Matrix const &b) {
	Matrix m;
	// if matrices are not the same size, return an empty matrix
	if (a.size() != b.size()) return m;
	int size = a.size();
	
	for (int i = 0; i < size; i++) {
		std::vector<int> row;
		for (int j = 0; j < size; j++) {
			row.push_back(a[i][j] + b[i][j]);
		}
		m.push_back(row);
	}
	return m;
}

Matrix matrix_multiply(const Matrix &a, const Matrix &b) {
	Matrix m;
	if (a.size() != b.size()) return m;
	int size = a.size();
	
	for (int i = 0; i < size; i++) {
		std::vector<int> row;
		for (int j = 0; j < size; j++) {
			// Calculate the dot product of each row-column pair
			int sum = 0;
			for (int k = 0; k < size; k++) {
				sum += a[i][k] * b[k][j];
			}
			row.push_back(sum);
			
		}
		m.push_back(row);
	}
	return m;
}

Matrix matrix_scale(const Matrix &a, const int s) {
	// multiply all values in a matrix by the same number
	Matrix m;
	int size = a.size();
	for (int i = 0; i < size; i++) {
		std::vector<int> row;
		for (int j = 0; j < size; j++) {
			row.push_back(a[i][j] * s);
		}
		m.push_back(row);
	}
	return m;
}

Matrix matrix_subtract(const Matrix &a, const Matrix &b) {
	return matrix_add(a, matrix_scale(b, -1));
}

bool matrix_set_value(Matrix &a, const unsigned int i, const unsigned int j, const int value) {
	// use the return value to tell whether the given location is out of range
	if (i >= a.size() || j >= a.size()) return false;
	a[i][j] = value;
	return true;
}

int matrix_max_value(const Matrix &a) {
	int max_value = a[0][0];
	int size = a.size();
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (a[i][j] > max_value) max_value = a[i][j];
		}
	}
	return max_value;
}

Matrix matrix_transpose(const Matrix &a) {
	Matrix m;
	int size = a.size();
	for (int i = 0; i < size; i++) {
		std::vector<int> row;
		for (int j = 0; j < size; j++) {
			// Switch the indices around when reading the original
			row.push_back(a[j][i]);
		}
		m.push_back(row);
	}
	return m;
}




int main() {
	auto file = std::fstream(INPUT_FILE_NAME, std::ios::in);
	
	if (!file.is_open()) {
		printf("Input file '%s' not found.\n", INPUT_FILE_NAME);
		return 0;
	}
	
	// This is way too simple to work for reading and parsing ints in my opinion, but it works
	int size;
	file >> size;
	
	if (size <= 0) {
		printf("Invalid matrix size: %d\n", size);
		return 0;
	}
	
	// Initialize the 2 matrices in an array so I can use the same block of code for reading into both
	Matrix m[2];
	int n;
	for (int k = 0; k < 2; k++) {
		for (int i = 0; i < size; i++) {
			// Store into each row
			std::vector<int> row;
			for (int j = 0; j < size; j++) {
				if (file.eof()) { // Check for unexpected end of file
					printf("Not enough values for two %dx%d matrices.\n", size, size);
					return 0;
				}
				
				file >> n;
				row.push_back(n);
			}
			// Finally store each row into the matrix
			m[k].push_back(row);
		}
	}
	
	file.close();
	
	// Call each implemented function on the matrices
	
	printf("\nMatrix A:\n");
	matrix_print(m[0]);
	printf("Matrix B:\n");
	matrix_print(m[1]);
	
	printf("\nA + B:\n");
	matrix_print(matrix_add(m[0], m[1]));
	
	printf("\nA * B:\n");
	matrix_print(matrix_multiply(m[0], m[1]));
	
	printf("\nA - B:\n");
	matrix_print(matrix_subtract(m[0], m[1]));
	
	printf("\nUpdate element of A: 1,1 => -101\n");
	matrix_set_value(m[0], 1, 1, -101);
	matrix_print(m[0]);
	
	printf("\nMaximum value of A: %d\n", matrix_max_value(m[0]));
	
	printf("\nTranspose of A:\n");
	matrix_print(matrix_transpose(m[0]));
	
	return 0;
}
