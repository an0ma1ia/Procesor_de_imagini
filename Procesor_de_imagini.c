    #include <stdio.h>
    #include <stdlib.h>
    #include "bmp_io_rgb.h"

    unsigned char ***square_format (unsigned char ***image, int N, int M, int *square_size) {

        *square_size = (N > M) ? N : M;
        unsigned char ***square = malloc (*square_size * sizeof (unsigned char **));
        
        if (square == NULL)
            return NULL;    

        int offset_line = (*square_size - N) / 2;
        int offset_collum = (*square_size - M) / 2;

        for (int i = 0; i < *square_size; i ++){
            square[i] = malloc (*square_size * sizeof (unsigned char *));
            if (square[i] == NULL)
                return NULL;

            for (int j = 0; j < *square_size; j ++){
                 square[i][j] = malloc (3 * sizeof (unsigned char));
                if (square[i][j] == 0)
                    return NULL;
                
                if (i >= offset_line && i < offset_line + N && j >= offset_collum && j < offset_collum + M) {
                    square[i][j][0] = image[i - offset_line][j - offset_collum][0];
                    square[i][j][1] = image[i - offset_line][j - offset_collum][1];
                    square[i][j][2] = image[i - offset_line][j - offset_collum][2];
                }  
                else {
                    square[i][j][0] = 255;
                    square[i][j][1] = 255;
                    square[i][j][2] = 255;
                }
            }  
        }
          return square;  
    }


    void average_rgb (unsigned char ***image, int N, int M) {   // grayscale
        for (int i = 0; i < N; i ++) {
            for (int j = 0; j < M; j ++){
                unsigned char r = image[i][j][0];
                unsigned char g = image[i][j][1];
                unsigned char b = image[i][j][2];

                unsigned char average = (r + g + b) / 3;

                image[i][j][0] = average;
                image[i][j][1] = average;
                image[i][j][2] = average;
            }   
        }
    }


    unsigned char ***cropped (unsigned char ***image, int N, int M, int left, int right, int top, int bottom, int *crop_N, int *crop_M) {
        *crop_N = bottom - top +1;
        *crop_M = right - left + 1;
        unsigned char ***cropped_image = malloc (*crop_N * sizeof (unsigned char **));

        if (cropped_image == NULL)
            return NULL;
        
            for (int i = 0; i < *crop_N; i ++) {
                cropped_image[i] = malloc (*crop_M * sizeof (unsigned char *));
                if (cropped_image[i] == NULL)
                    return NULL;
                for (int j = 0; j < *crop_M; j ++){
                    cropped_image[i][j] = malloc (3 * sizeof (unsigned char));
                if (cropped_image[i][j] == NULL)
                    return NULL;
                
                cropped_image[i][j][0] = image[top + i][left + j][0];
                cropped_image[i][j][1] = image[top + i][left + j][1];
                cropped_image[i][j][2] = image[top + i][left + j][2];
                }
            }
        return cropped_image;
    } 

    // functie care deseneaza un dreptunghi pe o imagine
    // dreptunghiul o sa apara si partial in imagine daca coordonatele sale depasesc dimensiunile imaginii 
    void rectangle (unsigned char ***image, int N, int M, int left, int right, int top, int bottom, int k) { 
        int a = (left > 0) ? left : 0;
        int b = (right < M) ? right : M - 1;
        int c = (top > 0) ? top : 0;
        int d = (bottom < N) ? bottom : N - 1;

        
        for (int j = c; j <= d; j++) {
            for (int i = a; i <= b; i++){
                if (i < left + k || i > right - k || j < top + k || j > bottom -k) {
                    image[j][i][0] = 255;
                    image[j][i][1] = 0;
                    image[j][i][2] = 0;
                }
            }
        }
    }

    int main () {

        //task 2
        int N, M;

        unsigned char ***image = bmp_read ("/home/amalia/Pictures/alpaca.bmp", &N, &M);
        if (image == NULL)
            return 1;


        // task 3
        // rectangle(image, N, M, 300, 550, 35, 510, 5);   // deseneaza un dreptunghi rosu de grosime k la coordonatele date
        // rectangle(image, N, M, 560, 750, 180, 470, 5);


        // task 4
        int crop_N, crop_M;
        //unsigned char ***cropped_image = cropped (image, N, M, 300, 550, 35, 510, &crop_N, &crop_M);   // da crop la dreptunghiul determinat de coordonatele date


        // task 5
        //average_rgb (image, N, M);   // fiecare pixel ia valoarea mediei dintre r, g, b (grayscale)


        // task 6
        int square_size;
        unsigned char ***square = square_format (image, N, M, &square_size);


        // afisarea imaginii
        // bmp_write ("/home/amalia/Diverse/Personal_projects/output.bmp", image, N, M);
        // bmp_write ("/home/amalia/Diverse/Personal_projects/cropped.bmp", cropped_image, crop_N, crop_M);
        // bmp_write ("/home/amalia/Diverse/Personal_projects/average.bmp", image, N, M);
        bmp_write ("/home/amalia/Diverse/Personal_projects/square.bmp", square, square_size, square_size);


         for (int i = 0; i < N; i ++) {
            for (int j = 0; j < M; j ++) 
                free (image[i][j]);
            free (image[i]);
        }
        free (image);

        // for (int i = 0; i < crop_N; i ++) {
        //     for (int j = 0; j < crop_M; j ++) 
        //         free (cropped_image[i][j]);
        //     free (cropped_image[i]);
        // }
        // free (cropped_image);

        for (int i = 0; i < square_size; i ++) {
            for (int j = 0; j < square_size; j ++) 
                free (square[i][j]);
            free (square[i]);
        }
        free (square);

        return 0;
    }