/*Program to Create a Naive Bayes classifier for each handwritten digit that support discrete and continuous features*/
/*Author: Siffi Singh */
/*Dated: 14/10/2017 */

/*Standard Headers */
#include<iostream>
#include<cmath>
#include<cstdio>
#include<cstring>
#include<vector>
#include<algorithm>
#include <fstream>
using namespace std;

/*Function to read files from MNIST dataset*/
int ReverseInt(int i) {
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 255;
    ch2 = (i >> 8) & 255;
    ch3 = (i >> 16) & 255;
    ch4 = (i >> 24) & 255;
    return ((int) ch1 << 24) + ((int) ch2 << 16) + ((int) ch3 << 8) + ch4;
}
void ReadMNIST(int NumberOfImages, int DataOfAnImage, vector < vector < double > > & arr, char s[]) {
    arr.resize(NumberOfImages, vector < double > (DataOfAnImage));
    ifstream file(s, ios::binary);
    if (file.is_open()) {
        int magic_number = 0;
        int number_of_images = 0;
        int n_rows = 0;
        int n_cols = 0;
        file.read((char * ) & magic_number, sizeof(magic_number));
        magic_number = ReverseInt(magic_number);
        file.read((char * ) & number_of_images, sizeof(number_of_images));
        number_of_images = ReverseInt(number_of_images);
        if (DataOfAnImage != 1) {
            file.read((char * ) & n_rows, sizeof(n_rows));
            n_rows = ReverseInt(n_rows);
            file.read((char * ) & n_cols, sizeof(n_cols));
            n_cols = ReverseInt(n_cols);
        }
        for (int i = 0; i < number_of_images; ++i) {
            if (DataOfAnImage != 1) {
                for (int r = 0; r < n_rows; ++r) {
                    for (int c = 0; c < n_cols; ++c) {
                        unsigned char temp = 0;
                        file.read((char * ) & temp, sizeof(temp));
                        arr[i][(n_rows * r) + c] = (double) temp;
                    }
                }
            } else {
                unsigned char temp = 0;
                file.read((char * ) & temp, sizeof(temp));
                arr[i][0] = (double) temp;
            }
        }
    }
}

/*Mapping values from 0-255 to 1-32*/
int convert_pixel(double x) {
    if (x >= 0 && x <= 7)
        return 1;
    else if (x >= 8 && x <= 15)
        return 2;
    else if (x >= 16 && x <= 23)
        return 3;
    else if (x >= 24 && x <= 31)
        return 4;
    else if (x >= 32 && x <= 39)
        return 5;
    else if (x >= 40 && x <= 47)
        return 6;
    else if (x >= 48 && x <= 55)
        return 7;
    else if (x >= 56 && x <= 63)
        return 8;
    else if (x >= 64 && x <= 71)
        return 9;
    else if (x >= 72 && x <= 79)
        return 10;
    else if (x >= 80 && x <= 87)
        return 11;
    else if (x >= 88 && x <= 95)
        return 12;
    else if (x >= 96 && x <= 103)
        return 13;
    else if (x >= 104 && x <= 111)
        return 14;
    else if (x >= 112 && x <= 119)
        return 15;
    else if (x >= 120 && x <= 127)
        return 16;
    else if (x >= 128 && x <= 135)
        return 17;
    else if (x >= 136 && x <= 143)
        return 18;
    else if (x >= 144 && x <= 151)
        return 19;
    else if (x >= 152 && x <= 159)
        return 20;
    else if (x >= 160 && x <= 167)
        return 21;
    else if (x >= 168 && x <= 175)
        return 22;
    else if (x >= 176 && x <= 183)
        return 23;
    else if (x >= 184 && x <= 191)
        return 24;
    else if (x >= 192 && x <= 199)
        return 25;
    else if (x >= 200 && x <= 207)
        return 26;
    else if (x >= 208 && x <= 215)
        return 27;
    else if (x >= 216 && x <= 223)
        return 28;
    else if (x >= 224 && x <= 231)
        return 29;
    else if (x >= 232 && x <= 239)
        return 30;
    else if (x >= 240 && x <= 247)
        return 31;
    else if (x >= 248 && x <= 255)
        return 32;

}

/*Driver Function*/
int main() {
	
	/* Variable declarations */
    vector < vector < double > > ar;
    char s1[] = "train-images.idx3-ubyte";
    char s2[] = "train-labels.idx1-ubyte";
    char s3[] = "t10k-images.idx3-ubyte";
    char s4[] = "t10k-labels.idx1-ubyte";
    vector < vector < double > > training_images; 
    vector < vector < double > > training_labels;
    vector < vector < double > > testing_images;
    vector < vector < double > > testing_labels;
    
	/*pixels: 784 = 28*28*/
    ReadMNIST(60000, 784, training_images, s1); //Storing training_images of size [60000, 784]
    for (int i = 0; i < 60000; i++) {
        for (int j = 0; j < 784; j++) {
            training_images[i][j] = convert_pixel(training_images[i][j]);
        }
    }
    ReadMNIST(60000, 1, training_labels, s2); //Storing training_labels of size [60000, 1]
    for (int i = 0; i < 60000; i++) {
    }
    ReadMNIST(10000, 784, testing_images, s3); //Storing testing_images of size [10000, 784]
    for (int i = 0; i < 10000; i++) {
        for (int j = 0; j < 784; j++) {
            testing_images[i][j] = convert_pixel(testing_images[i][j]);
        }
    }
    ReadMNIST(10000, 1, testing_labels, s4); //Storing testing_labels of size [10000, 1]
    for (int i = 0; i < 10000; i++) {
    }

    /*Calculating prior probabilities for classes 0-9*/
    int count[10] = {0};
    double Prior[10] = {0};
    for (int i = 0; i < 60000; i++) {
        if (training_labels[i][0] == 0)
            count[0]++;
        else if (training_labels[i][0] == 1)
            count[1]++;
        else if (training_labels[i][0] == 2)
            count[2]++;
        else if (training_labels[i][0] == 3)
            count[3]++;
        else if (training_labels[i][0] == 4)
            count[4]++;
        else if (training_labels[i][0] == 5)
            count[5]++;
        else if (training_labels[i][0] == 6)
            count[6]++;
        else if (training_labels[i][0] == 7)
            count[7]++;
        else if (training_labels[i][0] == 8)
            count[8]++;
        else if (training_labels[i][0] == 9)
            count[9]++;
    }	
    for (int i = 0; i <= 9; i++) {
        Prior[i] = (double) count[i] / 60000;
    }

	int toggle;
    cin >> toggle; //Toggle: 0 for Discrete Mode, 1 for Continuous Mode
       
    if (toggle == 0) {
    	
		/*Discrete Mode*/
        double LUT[10][784][32] = {0}, max = 0, sum = 0;
        int class_now;
        
        /*Creating Look-up-table(LUT) to compute probabilities of each value(1-32) for each pixel(1-784) and each class(0-9).*/
        for (int i = 0; i < 60000; i++) {
            for (int j = 0; j < 10; j++) {
                if (training_labels[i][0] == j)
                    class_now = j;
            }
            for (int j = 0; j < 784; j++) {
                LUT[class_now][j][(int) training_images[i][j]]++;
            }
        }
        
        /*Calculating Log Likelihood Probabilities*/
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 784; j++) {
                for (int k = 1; k <= 32; k++) {
                    LUT[i][j][k] = LUT[i][j][k] / count[i];	
                    //cout<<LUT[i][j][k]<<" ";
                }
            }
        }
        /*
        double MLE[10][784]={0}, SUM[10][784]={0};
        for(int i=0; i<60000; i++)
        {
        	for(int j=0; j<10; j++)
        	{
        		for(int k=0; k<784; k++)
        		{
        			if(training_labels[i][0]==j)
        			{
        				SUM[j][k]=SUM[j][k]+(training_images[i][k]);
					}
				}
			}
		}
		double LMLE[10]{0};
		for(int i=0; i<10; i++)
		{
			for(int j=0; j<784; j++)
			{
				MLE[i][j]=SUM[i][j]/count[i];
				LMLE[i]= LMLE[i]+(MLE[i][j]);
			}
			LMLE[i]=(log10(LMLE[i]))*Prior[i];
		}
		*/
		
		/*Calculating posterior probabilities for testing_images(1,10000), for each class(0-9)*/
        int ans = 0, error = 0;
        for (int i = 0; i < 10000; i++) {
            for (int j = 0; j < 10; j++) {
                for (int k = 0; k < 784; k++) {
                    sum = sum + LUT[j][k][(int) testing_images[i][k]];
                }
                /*Predicting class of the given input data by chossing the class with maximimum probability.*/
                if (sum >= max) {
                    max = sum;
                    ans = j;
                }
                sum = 0;
            }
            cout << ans << " " << testing_labels[i][0] << endl;
            if (ans == testing_labels[i][0])
                error++;
            max = 0;
        }
        
        /*Error Rate = (error/10000)*100*/
        cout << "Error rate: " << error / 100 << endl;
    } 
	else if (toggle == 1) {
		/*Continuous Mode*/
		
        /*Fitting guassian to data for each pixel(1-784) and each class(0-9)*/
        double SUM[10][784] = {0};
        int class_now;
        for (int i = 0; i < 60000; i++) {
            for (int j = 0; j < 10; j++) {
                if (training_labels[i][0] == j)
                    class_now = j;
            }
            for (int j = 0; j < 784; j++) {
                SUM[class_now][j] = SUM[class_now][j] + (training_images[i][j]);
            }
        }        
        double G[10][784][2]; //here G[][][0] stores mean values and G[][][1] stores variance values
        
        /*Calculating mean values for the guassian fitted*/
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 784; j++) {
                SUM[i][j] = SUM[i][j] / count[i];
                G[i][j][0] = SUM[i][j];
                SUM[i][j] = 0;
            }
        }
        
		/*VARSUM replaced by SUM*/
        double p; 
        for (int i = 0; i < 60000; i++) {
            for (int j = 0; j < 10; j++) {
                if (training_labels[i][0] == j)
                    class_now = j;
            }
            for (int j = 0; j < 784; j++) {
                p = (((training_images[class_now][j]) - (G[class_now][j][0])) * ((training_images[class_now][j]) - (G[class_now][j][0])));
                SUM[class_now][j] += p;
                //cout<<SUM[class_now][j]<<" ";			
            }
        }
        
        /*Calculating variance values for the guassian fitted*/
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 784; j++) {
                SUM[i][j] = SUM[i][j] / count[i]; 
                G[i][j][1] = SUM[i][j];
                //cout << G[i][j][1] << " ";
            }
        }
        
        /*Calculating posterior probabilities for testing_images(1,10000), for each class(0-9)*/
        double test_probs[10] = {0}, ans = 0, max = 0, error = 0, h;
        for (int i = 0; i < 10000; i++) {
            for (int j = 0; j < 10; j++) {
                for (int k = 0; k < 784; k++) {
                    /*using mean = G[j][k][0] & variance = G[j][k][1]
                    1/root(2*pi) = (0.3989422804)	*/
                    if ((G[j][k][1]) != 0) {
                        h = -(((testing_images[j][k] - G[j][k][0]) * (testing_images[j][k] - G[j][k][0]))) / (2 * (G[j][k][1] * G[j][k][1]));
                        test_probs[j] += (0.3989422804) * (1 / (G[j][k][1])) * exp(h);
                    } else {
                        h = -(((testing_images[j][k] - 0) * (testing_images[j][k] - 0))); //In cases where Variance =0, assuming distribution as ~N(0,1) distribution
                        test_probs[j] += (0.3989422804) * (1) * exp(h);
                    }                    																					
                }
                /*Predicting class of the given input data by chossing the class with maximimum probability.*/
                if(test_probs[j]>max)
                {
                	max = test_probs[j];
                	ans = j;
				}
                cout << j << " " << test_probs[j] << endl;
            }
            //cout<<ans<<" "<<testing_labels[i][0]<<endl;
            if (ans == testing_labels[i][0])
                error++;
            for (int j = 0; j < 10; j++) {
                test_probs[j] = 0;
            }
            max = 0;
        }
        cout << "Error: " << error << endl;

    }

    return 0;
}


