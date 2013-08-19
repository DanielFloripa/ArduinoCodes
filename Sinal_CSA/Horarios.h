#ifndef HORARIOS_H
#define	HORARIOS_H

#define QTD_HOR 20 //Sempre que adicionar um novo horário, incremente 2;
#define QTD_FER 20 //Sempre que adicionar um novo feriado, incremente 2;

int horario[QTD_HOR] = //hora, minuto,
        {8, 20, 
         8, 25,
         8, 30,
        13, 00,
        18, 00,
        19, 00,
        21, 00,
        21, 20,
        21, 40,
        21, 50};

int feriado[QTD_FER] = //dia, mês,
        {1, 1, //Dia da Paz
         9, 3, //Niver Joinville
        21, 4, //Tirandentes
         1, 5, //Trabalhador
         7, 9, //Independencia
        12, 10,//Nossa Senhora
         2, 11,//Finados
        15, 11,//República
        25, 12,//Natal
        31, 12};//Véspera Ano Novo

#endif

