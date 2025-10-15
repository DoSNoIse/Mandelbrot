#include <cmath>
#include <complex>
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>  // For formatting numbers
#include <sstream>  // For constructing file names
using namespace std;
using Complex = std::complex<long double>;
using namespace std::chrono;
// commit with terminal using: 
// git add .
// git commit -m "commit message here"
// git push origin main

const long int max_iterations = 1000;
const long int imagex = 1000;
const long int imagey = 1000;

const long double ax = 1;
const long double bx = -3;//   # 1000 It and 10000 x 9000 px
const long double ay = 1;
const long double by = -1;
// Compute Mandelbrot escape iterations
int mandel(double x, double y) {
    Complex c(x, y);
    Complex z = c;

    for (int i = 0; i < max_iterations; i++) {
        if (std::abs(z) > 2) {
            return i;
        }
        z = z * z + c;
    }

    return max_iterations;
}

string format_filename(milliseconds elapsed) { // time calculation
    long ms = elapsed.count();
    int hours = ms / (60 * 60000);
    int minutes = (ms / 60000) % 60;
    int seconds = (ms % 60000) / 1000;
    int milliseconds = ms % 1000;
    
    stringstream ss;
        ss << setw(2) << setfill('0') << hours << "hr"
        << setw(2) << setfill('0') << minutes << "m"
        << setw(2) << setfill('0') << seconds << "s"
        << setw(2) << setfill('0') << milliseconds << "ms.ppm";
    return ss.str();
}

void getColor(int iterations, int max_iterations, int &r, int &g, int &b) {
    double t = (double)iterations / max_iterations;  // Normalize to [0,1]

    t = pow(t, 5);  //! The lower the exponent, the stronger the contrast

    r = (int)(255 * cos(3.0 * M_PI * t));  // Sinusoidal function for wave-like contrast
    g = (int)(200 * pow(t, 0.4));          // Square root increases middle tones
    b = (int)(255 * pow(1 - t, 0.3));        // Exaggerates deep blues
    r = max(0, min(255, r));
    g = max(0, min(255, g));
    b = max(0, min(255, b));
    //sstd::cout<< "" << b << endl;
}

int main() {
    auto start_time = high_resolution_clock::now();

    long total_pixels = imagex * imagey;
    long halfway_point = total_pixels / 2;
    long quarter_left_point = 3 * total_pixels / 4;

    // Iterate through pixels and compute Mandelbrot fractal
    string temporary_filename = "temporary_output.ppm";
    ofstream myOutput(temporary_filename);
    myOutput.precision(15);
    myOutput << "P3\n" << imagex << " " << imagey << "\n255\n"; // PPM header

    for (long j = 0; j < imagey; j++) {
        double yz = j * (by - ay) / (imagey - 1) + ay;

        for (long i = 0; i < imagex; i++) {
            double xz = i * (bx - ax) / (imagex - 1) + ax;

            int iterations = mandel(xz, yz);
            int r, g, b;
            getColor(iterations, max_iterations, r, g, b);

            myOutput << r << " " << g << " " << b << " ";

            long current_pixel = j * imagex + i;
            if (current_pixel == halfway_point) {
                auto elapsed_time = duration_cast<milliseconds>(high_resolution_clock::now() - start_time);
                cout << "Be patient, we're halfway there! Time since first calculation: " << format_filename(elapsed_time) << "\n";
            }
            if (current_pixel == quarter_left_point) {
                auto elapsed_time = duration_cast<milliseconds>(high_resolution_clock::now() - start_time);
                cout << "Be patient, we're 3/4 there!,\n Time calculation: " << format_filename(elapsed_time) << "\n";
            }
        }
        myOutput << "\n";
    }
    
    auto elapsed_time = duration_cast<milliseconds>(high_resolution_clock::now() - start_time);
    myOutput.close();

    // Generate final filename with elapsed time
    string final_filename = format_filename(elapsed_time);
    
    // Rename temporary file to final filename
    rename(temporary_filename.c_str(), final_filename.c_str());

    cout << "Mandelbrot image saved as '" << final_filename << "'\n";

    return 0;
}
