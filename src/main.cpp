#include <iostream>
#include <fstream>
#include <windows.h>
#include <chrono>
using namespace std;
#define sizeOfBoard 50 
#define sizeOfWord 50
#define defaultChar '-'
int KToCheck(int k, int arah, int hurufKe);
int BToCheck(int b, int arah, int hurufKe);
int main(){
    // KAMUS
    int comparisonCounter = 0;
    char board[sizeOfBoard][sizeOfBoard];
    int boardFoundTracker[sizeOfBoard][sizeOfBoard];

    // 1. inisialisasi papan(yang diimplementasikan dalam bentuk array of char) untuk menyimpan tiap huruf,
    for(int b = 0 ; b < sizeOfBoard ; b++){
        for(int k = 0 ; k < sizeOfBoard ; k++){
            board[b][k] = defaultChar;
        }
    }

    //inisialisasi juga papan catatan lokasi ditemukannya word dalam papan.
    for(int b = 0 ; b < sizeOfBoard ; b++){
        for(int k = 0 ; k < sizeOfBoard ; k++){
            boardFoundTracker[b][k] = 0;
        }
    }

    //KAMUS
    string wordsToFind[sizeOfWord];
    string eachLine;
    ifstream puzzleText("../test/puzzle.txt");
    int baris = 0;
    int kolom;
    bool isboardDone = false;
    int wordCounter = 0;
    int foundCounter = 0;
    // 2. baca puzzle dari text(file yang digunakan disini adalah file dengan nama 'puzzle.txt'), lalu dimasukkan ke dalam array. 
    // implementasi ini bisa menerima file tanpa spasi, dengan 1 spasi, dengan 2 spasi, dst
    while(getline(puzzleText, eachLine)){
        if(eachLine.empty()){
            isboardDone = true;
        }

        if(isboardDone){
            //membaca words
            wordsToFind[wordCounter] = eachLine;
            wordCounter++;
        }
        else{
            //membaca board
            kolom = 0;
            for(int b = 0 ; b < eachLine.length(); b++){
                if(eachLine[b] != ' '){
                    board[baris][kolom] = eachLine[b];
                    kolom++;
                }
            }
            baris++;
        }
    }
    puzzleText.close();
    
    //3. lakukan pencarian seluruh kata di dalam papan.

    auto timeStart = chrono::high_resolution_clock::now();

    for(int loop = 0 ; loop < wordCounter; loop++){
        //4. untuk masing-masing kata yang dicari, lakukan iterasi kepada tiap huruf di papan.
        for(int b = 0 ; b < baris ; b++){
            for(int k = 0; k < kolom; k++){
                //5. jika huruf pertama sama pada word yang ingin dicari sama dengan huruf yang saat ini ditunjuk pada papan, 
                comparisonCounter++;
                if(wordsToFind[loop][0] == board[b][k]){
                    // lakukan pencarian ke 8 arah (seluruh petak di sekitar huruf tersebut).
                    bool isFound = false;
                    int arah = 1; //1 artinya kiri atas, 2 artinya atas. lebih mudah ilustrasi:
                    // 1 2 3
                    // 8 0 4
                    // 7 6 5
                    while(arah < 9 && !isFound){
                        char charInBoardToCompare;
                        bool sama = true;
                        int panjang = wordsToFind[loop].length();
                        int hurufKe = 1; //mulai dari 0. huruf ke 0 sudah di cek. selanjutnya huruf ke 1

                        //6. untuk masing-masing arah, selama seluruh huruf yang sudah dicek masih sama dan huruf yang dicek masih kurang dari panjang kata,
                        //  cek huruf berikutnya.
                        int bToCheck;
                        int kToCheck;
                        while(sama && hurufKe < panjang){
                            bToCheck = BToCheck(b,arah,hurufKe);
                            kToCheck = KToCheck(k,arah,hurufKe);

                            //cek apakah lokasi huruf dalam papan yang akan dicek valid(tidak valid jika melebihi batas kiri kanan atas bawah.), jika tidak berarti tidak mungkin ketemu.
                            if(bToCheck < 0 || kToCheck < 0 || bToCheck >= baris || kToCheck >= kolom){
                                sama = false;
                            }
                            else{
                                comparisonCounter++;
                                if(wordsToFind[loop][hurufKe] != board[bToCheck][kToCheck]){
                                    sama = false;
                                }
                            }
                            hurufKe++;
                        }
                        //7. jika panjang huruf sudah melebihi panjang kata dan masih sama, artinya setiap huruf pada kata tersebut sama, artinya, word telah ditemukan di dalam papan.
                        if(sama){
                            isFound = true;
                            foundCounter++;
                            // tandai boardFoundTracker
                            for(int karakterKe = 0; karakterKe < panjang ; karakterKe++){
                                //8. catat lokasi ditemukannya kata tersebut.
                                //kalau ada kata yang digunakan bersama, prioritasin word yang lebih dulu dicari
                                if(boardFoundTracker[BToCheck(b,arah,karakterKe)][KToCheck(k,arah,karakterKe)] == 0){
                                    boardFoundTracker[BToCheck(b,arah,karakterKe)][KToCheck(k,arah,karakterKe)] = loop;
                                }
                            }
                        }
                        arah++;
                    }  
                }
            }
        }

    } // selesai mengecek seluruh kata dalam papan
    //9. cetak papan secara lengkap. jika huruf pada papan merupakan salah satu bagian huruf dari kata yang dicari, print huruf dengan warna. 
    cout << "=== HASIL ===" << endl;
    for(int b = 0 ; b < baris ; b++){
        for(int k = 0 ; k < kolom ; k++){
            string colorCode;
            if(boardFoundTracker[b][k] != 0){
                colorCode = "\033[3" +to_string((boardFoundTracker[b][k] % 6) + 1) + "m";
            }
            else{
                colorCode = "\033[37m";

            }
            cout <<  colorCode << board[b][k] << "\033[0m" << " ";
        }
        cout << endl;
    }
    // 10. print detail seperti waktu yang dibutuhkan dan total perbandingan.
    cout << "=== DETAIL ===" << endl;
    cout << "baris = " << baris << endl;
    cout << "kolom = " << kolom << endl;
    cout << "total kata = " << wordCounter - 1 << endl;
    cout << "total kata yang ditemukan di dalam papan = " << foundCounter << endl;
    auto timeEnd = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(timeEnd - timeStart);
    cout << "Time taken = "<< duration.count() << " microseconds" << endl;
    cout << "total comparison = " << comparisonCounter << endl;
}
//FUNGSI DAN PROSEDUR TAMBAHAN
int KToCheck(int k, int arah, int hurufKe){
    int kToCheck;
    if(arah == 1 || arah == 8 || arah == 7){
        kToCheck = k - hurufKe;
    }
    if(arah == 2 || arah == 6){
        kToCheck = k;
    }
    if(arah == 3 || arah == 4 || arah == 5){
        kToCheck = k + hurufKe;
    }
    return kToCheck;
}
int BToCheck(int b, int arah, int hurufKe){
    int bToCheck;
    if(arah == 1 || arah == 2 || arah == 3){
        bToCheck = b - hurufKe;
    }
    if(arah == 4 || arah == 8){
        bToCheck = b;
    }
    if(arah == 7 || arah == 6 || arah == 5){
        bToCheck = b + hurufKe;
    }
    return bToCheck;
}