#include <iostream>
#include <fstream>
#include <string>
#include <uchar.h>
using namespace std;
typedef unsigned char uchar;

auto reverseInt = [](int i)
{
	unsigned char c1, c2, c3, c4;
	c1 = i & 255, c2 = (i >> 8) & 255, c3 = (i >> 16) & 255, c4 = (i >> 24)
		& 255;
	return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
};

uchar** read_mnist_images(string full_path, int& number_of_images, int&
	image_size)
{
	typedef unsigned char uchar;
	ifstream file(full_path, ios::binary);
	if (file.is_open())
	{
		int magic_number = 0, n_rows = 0, n_cols = 0;
		file.read((char*)&magic_number, sizeof(magic_number));
		magic_number = reverseInt(magic_number);
		if (magic_number != 2051) throw runtime_error("Invalid MNIST imagefile!");
			file.read((char*)&number_of_images, sizeof(number_of_images)),
			number_of_images = reverseInt(number_of_images);
		file.read((char*)&n_rows, sizeof(n_rows)), n_rows = reverseInt
		(n_rows);
		file.read((char*)&n_cols, sizeof(n_cols)), n_cols = reverseInt
		(n_cols);
		image_size = n_rows * n_cols;
		uchar * *_dataset = new uchar * [number_of_images];
		int row = 0;
		int clmn = 0;
		for (int i = 0; i < number_of_images; i++)
		{
			_dataset[i] = new uchar[image_size];
			file.read((char*)_dataset[i], image_size);
			int index = 0;
			for (int row = 0; row < n_rows; row++)
			{
				for (int clmn = 0; clmn < n_cols; clmn++)
				{
					int temp = _dataset[i][index] & 0xFF;
					//printf("%d ", temp);
					index++;
				}
				//cout << endl << endl;
			}
			//cout << "*********************************\n";
		}
		return _dataset;
	}
	else
	{
		throw runtime_error("Cannot open file `" + full_path + "`!");
	}
}

uchar* read_mnist_labels(string full_path, int& number_of_labels)
{
	typedef unsigned char uchar;
	ifstream file(full_path, ios::binary);
	if (file.is_open())
	{
		int magic_number = 0;
		file.read((char*)&magic_number, sizeof(magic_number));
		magic_number = reverseInt(magic_number);
		if (magic_number != 2049) throw runtime_error("Invalid MNIST labelfile!");
			file.read((char*)&number_of_labels, sizeof(number_of_labels)),
			number_of_labels = reverseInt(number_of_labels);
		uchar * _dataset = new uchar[number_of_labels];
		for (int i = 0; i < number_of_labels; i++)
		{
			file.read((char*)&_dataset[i], 1);
			int temp = _dataset[i] & 0xFF;
			cout << temp << endl;
		}
		return _dataset;
	}
	else
	{
		throw runtime_error("Unable to open file `" + full_path + "`!");
	}
}

int main() {
	uchar* MNIST_labels, ** MNIST_images;
	string MNIST_path_labels = "train-labels.idx1-ubyte";
	string MNIST_path_images = "train-images.idx3-ubyte";

	string MNIST_TEST_path_labels = "t10k-labels.idx1-ubyte";
	string MNIST_TEST_path_images = "t10k-images.idx3-ubyte";
	int number_of_data;
	MNIST_labels = read_mnist_labels(MNIST_TEST_path_labels, number_of_data);
	int numbers_of_images, images_size;
	MNIST_images = read_mnist_images(MNIST_TEST_path_images, numbers_of_images,
		images_size);
	ofstream fout;
	fout.precision(3);
	fout.open("lib_10k.txt");
	for (int i = 0; i < number_of_data; i++) {
		int tmp1 = MNIST_labels[i] & 0xFF;
		fout << tmp1 << endl;
		int index = 0;
		for (int j = 0; j < 28; j++) {
			for (int k = 0; k < 28; k++) {
				double temp = (MNIST_images[i][index] & 0xFF)/255.;
				fout << temp << " ";
				index++;
			}
			fout << endl;
		}
	}
	return 0;
}
