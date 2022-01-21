# digunakan buat ngerapiin file input
print("gunakan image to text yang ada di online. aku coba pake 3 biji online image to text, spasi sama enter jadi jelek")
print("pake program ini buat ngerapiin")
print("file akan diambil dari file 'toconvert.txt' hasil akan ditunjukkan dilayar dan dituliskan ke dalam file 'doneconvert.txt'")

f = open("toconvert.txt", "r")
all = f.read()
all = all.replace(" ", "")
all = all.replace("\n", "")

print("ukuran puzzle small(1), medium(2), large(3)")
ukuran = input()
baris = 0
kolom = 0
if (ukuran == '1'):
    baris = 14
    kolom = 16
if (ukuran == '2'):
    baris = 20
    kolom = 22
if (ukuran == '3'):
    baris = 32
    kolom = 34

hasil = ""
counter = 0
for b in range (baris):
    for k in range (kolom):
        hasil = hasil + all[counter]
        counter += 1
        if (k != kolom - 1):
            hasil = hasil + ' '
    hasil = hasil + '\n'
print("hasil")
print(hasil)

f = open("doneconvert.txt", "a")
f.write(hasil)
f.close