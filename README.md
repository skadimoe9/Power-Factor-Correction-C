# Power-Factor-Correction-C
Yang diinput : 
P 
S
Jenis beban
Target PF : >0.85 <1

Program : 
- Pake struct buat input P, S, jenis beban, dan target PF 
- Cari Qhitung1 =  (sqrt((S*S)-(P*P*))
- PF lama = P/S 
- PF lama < PF target? jika ya, jalankan program
- Cari Sudut PF Target = arccos(PF Target) 
- Cari Qhitung2 = Tan(Sudut PF Target) * P 
- Cari Qselisih = Qhitung1 - Qhitung2 
- Cari reaktansi induktif atau kapasitif XL atau XC = V/Qselisih 
- Buat branch berdasarkan jenis beban Induktif atau Kapasitif (Pakai if statement) 
- Kalo induktif cari induktansi L = XL/2.phi.f (f = 50 Hz) 
- Kalo kapasitif cari kapasitansi C = 1/2.phi.f.XC 
