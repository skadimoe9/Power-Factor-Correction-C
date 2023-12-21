// Factory Power Factor Correction Program - Final Project
// Fadhli Ammar Taqiyuddin Hakim, 2206817396, Grup 07
// Mochamad Raihan Triadi, 2206814002, Grup 07
// Created on 05/12/2023
// Versi 2.0, Revised on 13/12/2023

#include <stdio.h>
#include <math.h> 
#include <stdlib.h> 

typedef struct{
	char nama_mesin[50]; 
	double P, S, Q, PF, P_Target, Q_Target, reaktansi, komponen;
	int jenis_beban; 
}PFC; 

void introText(); // Intro text pada program
void inputData(PFC *data_mesin, int *i); // Untuk memasukkan data mesin
void printPF(PFC *data_mesin, int *i); // Untuk melakukan cek pada Power Factor (PF) apakah sudah sesuai standar, jika belum maka PFC akan dilakukan 
void validateMesin(PFC *data_mesin, int jumlah_mesin); // Melakukan proses input dan cek Power Factor di satu function
void reactivePower(PFC *data_mesin, int *i); // Mencari daya reaktif dari mesin 
void processPFC(PFC *data_mesin, int jumlah_mesin, double V, double F, double TAN); // Proses penghitungan nilai reaktansi dan nilai dari komponen reaktifnya (induktansi/kapasitansi) 
void printKomponen(PFC *data_mesin, int jumlah_mesin); // Proses untuk mendisplay komponen reaktif yang diperlukan mesin
void informationMenu(); // Menu untuk informasi program

int main(){
	PFC *data_mesin; 
	const double TARGET_PF = 0.9; // Library math.h sudah memiliki nilai PI (3.14...), tinggal dipanggil saja lewat M_PI
	const double TAN_FASA_TARGET = tan(acos(0.9)); // Tan dari beda fasa PF target, untuk cari Daya Reaktif mesin saat PF 0.9
	const double TEGANGAN = 220, FREKUENSI = 50;
	int jumlah_mesin = -1, choice, choice_2; 
	char c;
	
	do{
		introText();
		do{
			printf("Pilihan anda : ");
			scanf("%d", &choice); 
			if(choice > 3 || choice < 1){
			printf("\nTolong masukkan pilihan yang valid!\n");
			}
		}while(choice > 3 || choice < 1);
		
		if(choice == 1){
			informationMenu();
			do{
				printf("\nKetik 0 untuk kembali ke menu : ");
				scanf("%d", &choice_2);
				if(choice_2 != 0){
					printf("\nTolong masukkan pilihan yang valid!\n");
				}
			}while(choice_2 != 0); 
			printf("\n");
			continue;
		}
		
		if(choice == 2){
			printf("========================================================================================================\n");
			if(jumlah_mesin == -1){
				printf("Silahkan masukkan jumlah mesin untuk Power Factor Correction\n"); 	
				printf("\nJumlah Mesin : ");
				while(scanf("%d", &jumlah_mesin) != 1)
			    {
			        printf("Masukkan angka yang valid! \n\nJumlah Mesin : ");
			        scanf("%d", &jumlah_mesin);
			        while ((c = getchar()) != '\n' && c != EOF); // Flush stdin 
			    }
			    printf("\n========================================================================================================\n"); 
			}
			printf("Proses Input Data Mesin\n");
			
			data_mesin = malloc(jumlah_mesin*sizeof(PFC)); 
			
			validateMesin(data_mesin, jumlah_mesin); // Proses input dan pengecekan faktor daya
			
			processPFC(data_mesin, jumlah_mesin, TEGANGAN, FREKUENSI, TAN_FASA_TARGET); // Proses perbaikan faktor daya dengan menentukan nilai komponen reaktif yang perlu ditambah
			printKomponen(data_mesin, jumlah_mesin);
		
			free(data_mesin); 
			do{
				printf("\nKetik 0 untuk kembali ke menu : ");
				scanf("%d", &choice_2);
				if(choice_2 != 0){
					printf("\nTolong masukkan pilihan yang valid!\n");
				}
			}while(choice_2 != 0);  
			printf("\n");
			continue;
		} 
		
		if(choice == 3){
			printf("\nTerima kasih telah menggunakan program kami!\n");
			getch();
		}
	}while(choice != 3);
	return 0;
}

void introText(){ // Intro program di awal

	printf("=================================== Factory Power Factor Correction ====================================\n");
	printf(	"Selamat datang di PFC Program! Silahkan pilih berdasarkan menu berikut : \n" 
			"1 - Informasi Program \n2 - Jalankan Program \n3 - Keluar dari Program\n");
	
}
void informationMenu(){
	printf("========================================================================================================\n");
	printf(	"Selamat datang di menu informasi, berikut penjelasan terkait PFC Program"
			"\nProgram ini akan membantu anda untuk memberikan nilai dari komponen reaktif \n"
			"yang diperlukan agar faktor daya mesin anda mencapai 0.9 (di atas standar PLN)\n\n");
	printf(	"Suplai tegangan menggunakan standar yang , digunakan di Indonesia - 220 V \n"
			"Frekuensi yang dipakai menggunakan standar yang digunakan di Indonesia - 50 Hz\n\n"); 
	printf(	"Daya Aktif (P) = Jumlah daya yang dipakai oleh mesin untuk bekerja \n"
			"Daya Semu (S) = Jumlah daya yang disuplai oleh sumber penyedia listrik \nDaya"
			" Reaktif (Q) = Jumlah daya yang diserap oleh komponen reaktif"" pada mesin\n\n"); 
	printf("========================================================================================================\n");
	}
void inputData(PFC *data_mesin, int *i){ // Memasukkan nilai-nilai dari variabel pada struct
	
	printf("\nSilahkan masukkan data mesin ke-%d \n", (*i)+1);
	printf("Nama Mesin %d : ", (*i)+1);  
	scanf(" %[^\n]s", &data_mesin[*i].nama_mesin);
	
	printf("Daya Semu (S) Mesin %d (VA) : ", (*i)+1);
	scanf("%lf", &data_mesin[*i].S); 
	
	do{
		printf("Daya Aktif (P) Mesin %d (Watt) : ", (*i)+1);  
		scanf("%lf", &data_mesin[*i].P); 
		if(data_mesin[*i].S < data_mesin[*i].P){
			printf("\nNilai Daya Semu (S) harus lebih besar dari Daya Aktif (P)!\n\n");
		}
	}while(data_mesin[*i].S < data_mesin[*i].P); // Daya Semu harus lebih besar dari Daya Aktif
	
	do{
		printf("Jenis Beban Mesin %d (1 - Induktif, 2 - Kapasitif) : ", (*i)+1); 
		scanf("%d", &data_mesin[*i].jenis_beban); 
		if(data_mesin[*i].jenis_beban != 1 && data_mesin[*i].jenis_beban != 2){
			printf("\nPilihan tidak valid!\n");
		}
	}while(data_mesin[*i].jenis_beban != 1 && data_mesin[*i].jenis_beban != 2); // Di luar dari pilihan 1 dan 2 gak bakal bisa 
	
}
void printPF(PFC *data_mesin, int *i){ 
	
	data_mesin[*i].PF = data_mesin[*i].P/data_mesin[*i].S; 
	
	if(data_mesin[*i].PF >= 0.9 && data_mesin[*i].PF < 1){
		switch(data_mesin[*i].jenis_beban){
			case 1 : 
				printf(	"\nFaktor Daya (PF) Mesin %d = %.3lf (Induktif) "
						"\nDaya Reaktif (Q) Mesin %d = %.3lf VAR\n", (*i)+1, data_mesin[*i].PF, (*i)+1, data_mesin[*i].Q);
				break; 
			case 2 : 
				printf(	"\nFaktor Daya (PF) Mesin %d = %.3lf (Kapasitif) "
						"\nDaya Reaktif (Q) Mesin %d = %.3lf VAR\n", (*i)+1, data_mesin[*i].PF, (*i)+1, data_mesin[*i].Q);
				break; 
			default : 
				break;
		}
		printf("\nFaktor Daya telah memenuhi standar minimum dari program ini (PF >= 0.9) \n", data_mesin[*i].PF); 
		printf(	"Power Factor Correction tidak akan dilakukan untuk mesin '%s' karena "
				"Faktor Daya telah memenuhi standar minimum\n", data_mesin[*i].nama_mesin);
	}
	else if(data_mesin[*i].PF < 0.9){
		switch(data_mesin[*i].jenis_beban){
			case 1 : 
				printf(	"\nFaktor Daya (PF) Mesin %d = %.3lf (Induktif) "
						"\nDaya Reaktif (Q) Mesin %d = %.3lf VAR\n", (*i)+1, data_mesin[*i].PF, (*i)+1, data_mesin[*i].Q); 
				break; 
			case 2 : 
				printf(	"\nFaktor Daya (PF) Mesin %d = %.3lf (Kapasitif) "
						"\nDaya Reaktif (Q) Mesin %d = %.3lf VAR\n", (*i)+1, data_mesin[*i].PF, (*i)+1, data_mesin[*i].Q); 
				break; 
			default : 
				break;
		}
		printf("\nFaktor Daya belum memenuhi standar minimum dari program ini (PF < 0.9) \n");
		printf(	"Power Factor Correction akan dilakukan untuk mesin '%s' "
				"agar Faktor Daya dapat mencapai standar minimum\n", data_mesin[*i].nama_mesin);
	}
	else if(data_mesin[*i].PF = 1){
		printf(	"\nFaktor Daya (PF) Mesin %d = %.3lf (Resistif) "
				"\nDaya Reaktif (Q) Mesin %d = %.3lf VAR\n", (*i)+1, data_mesin[*i].PF, (*i)+1, data_mesin[*i].Q);
		printf("\nFaktor Daya telah memenuhi standar minimum dari program ini (PF >= 0.9) \n", data_mesin[*i].PF); 
		printf(	"Power Factor Correction tidak akan dilakukan untuk mesin '%s' karena "
				"Faktor Daya telah memenuhi standar minimum\n", data_mesin[*i].nama_mesin);
	}
}
void reactivePower(PFC *data_mesin, int *i){
	data_mesin[*i].Q = sqrt(pow(data_mesin[*i].S, 2) - pow(data_mesin[*i].P, 2)); 
}
void validateMesin(PFC *data_mesin, int jumlah_mesin){
	int i; 
	
	for(i = 0; i < jumlah_mesin; i++){
		inputData(data_mesin, &i); 
		reactivePower(data_mesin, &i);
		printPF(data_mesin, &i); 
		printf("========================================================================================================\n");
	}
} 
void processPFC(PFC *data_mesin, int jumlah_mesin, double V, double F, double TAN){
	
	int i; 
	
	for(i = 0; i < jumlah_mesin; i++){
		if(data_mesin[i].PF >= 0.9 && data_mesin[i].PF <= 1){
			data_mesin[i].Q_Target = data_mesin[i].Q; 
			data_mesin[i].P_Target = data_mesin[i].P; 
			data_mesin[i].reaktansi = 0; 
			data_mesin[i].komponen = 0;
		}
		else if(data_mesin[i].PF < 0.9){
			data_mesin[i].Q_Target = TAN * data_mesin[i].P; // Nentuin nilai Q_Target (nilai Q saat PF dari mesinnya adalah 0.9) 
			data_mesin[i].reaktansi = V/(data_mesin[i].Q - data_mesin[i].Q_Target); // Mencari nilai reaktansi induktif/kapasitif (XL atau XC)
			data_mesin[i].P_Target = sqrt(pow(data_mesin[i].S, 2) - pow(data_mesin[i].Q_Target, 2));
			
			if(data_mesin[i].jenis_beban == 1){ // Jika beban induktif, kita cari nilai kapasitansi yang perlu ditambahkan 
				data_mesin[i].komponen = (1/(2*M_PI*F*data_mesin[i].reaktansi))*10000; 
			}
			else if(data_mesin[i].jenis_beban == 2){ // Jika beban kapasitif, kita cari nilai induktansi yang perlu ditambahkan 
				data_mesin[i].komponen = (data_mesin[i].reaktansi/(2*M_PI*F))*10000; 
			}
		}
	}
	
} 
void printKomponen(PFC *data_mesin, int jumlah_mesin){
	int i; 
	
	printf("Power Factor Correction\n");
	
	for(i = 0; i < jumlah_mesin; i++){
		if(data_mesin[i].PF < 0.9){
			if(data_mesin[i].jenis_beban == 1){  
				printf("\nMesin ke-%d '%s' perlu ditambahkan kapasitor dengan "
						"nilai kapasitansi sebesar %lf x 10^(-4) Farad\n", i+1, data_mesin[i].nama_mesin, data_mesin[i].komponen); 
				printf(	"Setelah Power Factor Correction, Faktor Daya menjadi 0.9 dengan perubahan nilai : "
						"\nDaya Aktif (P) = %.3lf Watt\nDaya Reaktif = %.3lf VAR\n\n", data_mesin[i].P_Target, data_mesin[i].Q_Target); 
				
			}
			else if(data_mesin[i].jenis_beban == 2){ // Jika beban kapasitif, kita cari nilai induktansi yang perlu ditambahkan 
				printf(	"\nMesin ke-%d '%s' perlu ditambahkan induktor dengan "
						"nilai induktansi sebesar %lf x 10^(-4) Henry\n", i+1, data_mesin[i].nama_mesin, data_mesin[i].komponen); 
				printf(	"Setelah Power Factor Correction, Faktor Daya menjadi 0.9 dengan perubahan nilai : "
						"\nDaya Aktif (P) = %.3lf Watt\nDaya Reaktif = %.3lf VAR\n\n", data_mesin[i].P_Target, data_mesin[i].Q_Target);  
			}
		}
			
		else if(data_mesin[i].PF >= 0.9 && data_mesin[i].PF <= 1){
			printf(	"\nMesin ke-%d '%s' tidak memerlukan Power Factor Correction, "
					"karena Faktor Daya-nya sudah memenuhi standar minimum \n(PF >= 0.9)\n\n", i+1, data_mesin[i].nama_mesin); 
		}
			
			printf("========================================================================================================\n");
	} 
}
