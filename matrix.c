#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int **creer_matrice(int dim)
{
    int **matrice;

    matrice = malloc(dim * sizeof(int *));
    if (matrice == NULL)
    {
        perror("Erreur malloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dim; i++)
    {
        matrice[i] = malloc(dim * sizeof(int));

        if (matrice[i] == NULL)
        {
            perror("Erreur malloc");

            for (int j = 0; j < i; j++)
                free(matrice[j]);

            free(matrice);
            exit(EXIT_FAILURE);
        }
    }

    /*
     * Remplissage aléatoire de la matrice  par des nombres 
     */
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            matrice[i][j] = rand() % 100;
        }
    }

    return matrice;
}

void liberer_matrice(int **matrice, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        free(matrice[i]);
    }

    free(matrice);
}


void enregistrer_binaire(char *nomFichier, int **matrice, int dim)
{
    int fd;

    //Ouverture  et création du fichier
     
    fd = open(nomFichier, O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (fd == -1)
    {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    /*
     * Récupération des informations du fichier
     */
    struct stat st;

    if (fstat(fd, &st) == -1)
    {
        perror("Erreur fstat");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /*
     * Taille du buffer :
     * on utilise la taille de bloc du fichier.
     */
    char *buff = malloc(st.st_blksize);

    if (buff == NULL)
    {
        perror("Erreur malloc");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /*
     * Comme notre matrice contient des int,
     * on remplit le buffer avec les octets
     * de la matrice.
     */
    int position = 0;

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            /*
             * Si un int ne peut pas entrer entièrement
             * dans le buffer restant, on écrit d'abord
             * le contenu du buffer.
             */
            if (position + sizeof(int) > st.st_blksize)
            {
                ssize_t nbwrite = write(fd, buff, position);

                if (nbwrite == -1)
                {
                    perror("Erreur write");
                    free(buff);
                    close(fd);
                    exit(EXIT_FAILURE);
                }

                position = 0;
            }

            /*
             * Copie de l'entier dans le buffer.
             */
            memcpy(buff + position,
                   &matrice[i][j],
                   sizeof(int));

            position += sizeof(int);
        }
    }

    /*
     * Ecriture du dernier bloc
     * s'il reste des données dans le buffer.
     */
    if (position > 0)
    {
        ssize_t nbwrite = write(fd, buff, position);

        if (nbwrite == -1)
        {
            perror("Erreur write");
            free(buff);
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    free(buff);
    close(fd);

    printf("Matrice enregistrée dans %s\n", nomFichier);
}



void afficher_binaire(char *nomFichier, int dim)
{
    int fd;

    // Ouverture du fichier en lecture seule
    fd = open(nomFichier, O_RDONLY);

    if (fd == -1)
    {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Récupération des informations du fichier
     
    struct stat st;

    if (fstat(fd, &st) == -1)
    {
        perror("Erreur fstat");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /*
     * Création du buffer selon la taille
     * de bloc du fichier.
     */
    char *buff = malloc(st.st_blksize);

    if (buff == NULL)
    {
        perror("Erreur malloc");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /*
     * Nombre d'octets lus.
     */
    ssize_t nbread;

    /*
     * position permet de savoir où nous sommes
     * dans la matrice.
     */
    int position = 0;

    /*
     * Lecture du fichier par blocs.
     */
    nbread = read(fd, buff, st.st_blksize);

    while (nbread > 0)
    {
        /*
         * On parcourt le buffer par sizeof(int),
         * car les données du fichier sont des int.
         */
        for (int i = 0;
             i + sizeof(int) <= nbread;
             i += sizeof(int))
        {
            int valeur;

            /*
             * Copier un entier du buffer vers une variable.
             */
            memcpy(&valeur,
                   buff + i,
                   sizeof(int));

            printf("%4d", valeur);

            position++;

            if (position % dim == 0)
            {
                printf("\n");
            }
        }

         
        nbread = read(fd, buff, st.st_blksize);
    }

    // Vérification de l'erreur de lecture.
     
    if (nbread == -1)
    {
        perror("Erreur read");
        free(buff);
        close(fd);
        exit(EXIT_FAILURE);
    }

    free(buff);
    close(fd);
}


void enregistrer_texte(char *nomFichier, int **matrice, int dim)
{
    int fd;

    // Création du fichier texte
     
    fd = open(nomFichier, O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (fd == -1)
    {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    //Récupération des informations du fichier.
     
    
    struct stat st;

    if (fstat(fd, &st) == -1)
    {
        perror("Erreur fstat");
        close(fd);
        exit(EXIT_FAILURE);
    }

   
     
    char *buff = malloc(st.st_blksize);

    if (buff == NULL)
    {
        perror("Erreur malloc");
        close(fd);
        exit(EXIT_FAILURE);
    }

    int position = 0;

    // Parcours de la matrice
    
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            char chaine[20];

            // Transformation de l'entier en texte.
             
            int longueur = snprintf( chaine,sizeof(chaine),"%d ",matrice[i][j]);

            
            if (position + longueur > st.st_blksize)
            {
                ssize_t nbwrite = write(fd, buff, position);

                if (nbwrite == -1)
                {
                    perror("Erreur write");
                    free(buff);
                    close(fd);
                    exit(EXIT_FAILURE);
                }

                position = 0;
            }

            
            memcpy(buff + position, chaine, longueur);

            position += longueur;
        }

       

        if (position + 1 > st.st_blksize)
        {
            ssize_t nbwrite = write(fd, buff, position);

            if (nbwrite == -1)
            {
                perror("Erreur write");
                free(buff);
                close(fd);
                exit(EXIT_FAILURE);
            }

            position = 0;
        }

        buff[position] = '\n';
        position++;
    }

    
    if (position > 0)
    {
        ssize_t nbwrite = write(fd, buff, position);

        if (nbwrite == -1)
        {
            perror("Erreur write");
            free(buff);
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    free(buff);
    close(fd);

    printf("Matrice enregistrée en texte dans %s\n", nomFichier);
}


void afficher_texte(char *nomFichier)
{
    int fd;

    fd = open(nomFichier, O_RDONLY);

    if (fd == -1)
    {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    struct stat st;

    if (fstat(fd, &st) == -1)
    {
        perror("Erreur fstat");
        close(fd);
        exit(EXIT_FAILURE);
    }

    char *buff = malloc(st.st_blksize);

    if (buff == NULL)
    {
        perror("Erreur malloc");
        close(fd);
        exit(EXIT_FAILURE);
    }

    ssize_t nbread;

    // Lecture par blocs

    nbread = read(fd, buff, st.st_blksize);

    while (nbread > 0)
    {
        ssize_t total = 0;

        
        while (total < nbread)
        {
            ssize_t nbwrite = write(
                STDOUT_FILENO,
                buff + total,
                nbread - total
            );

            if (nbwrite == -1)
            {
                perror("Erreur write");
                free(buff);
                close(fd);
                exit(EXIT_FAILURE);
            }

            total += nbwrite;
        }

        nbread = read(fd, buff, st.st_blksize);
    }

    if (nbread == -1)
    {
        perror("Erreur read");
        free(buff);
        close(fd);
        exit(EXIT_FAILURE);
    }

    free(buff);
    close(fd);
}


int main(int argc, char const *argv[])
{    

    int opt;

    int creation = 0;
    int affichage = 0;
    int dim = 0;

    int binaire = 1;   
    int texte = 0;

    char *nomFichier = NULL;


    
    while ((opt = getopt(argc, argv, "cad:btf:")) != -1)
    {
        switch (opt)
        {
            case 'c':
                creation = 1;
                break;

            case 'a':
                affichage = 1;
                break;

            case 'd':
                dim = atoi(optarg);
                break;

            case 'b':
                binaire = 1;
                texte = 0;
                break;

            case 't':
                texte = 1;
                binaire = 0;
                break;

            case 'f':
                nomFichier = optarg;
                break;

            default:
                printf("Usage : %s -c|-a -d dimension -f fichier [-b|-t]\n",
                       argv[0]);

                return 0;
        }
    }


    // Vérification des options
     

    if (creation == 0 && affichage == 0)
    {
        printf("Erreur : il faut utiliser -c ou -a\n");
        return 0;
    }

    if (creation == 1 && affichage == 1)
    {
        printf("Erreur : -c et -a ne peuvent pas être utilisés ensemble\n");
        return 0;
    }

    if (dim <= 0)
    {
        printf("Erreur : la dimension doit être positive\n");
        return 0;
    }

    if (nomFichier == NULL)
    {
        printf("Erreur : vous devez préciser un fichier avec -f\n");
        return 0;
    }


    srand(time(NULL));

    if (creation)
{
    int **matrice;

    matrice = creer_matrice(dim);

    printf("Matrice %dx%d :\n", dim, dim);

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            printf("%4d", matrice[i][j]);
        }

        printf("\n");
    }

    if (binaire)
    {
        enregistrer_binaire(nomFichier, matrice, dim);
    }
    else
    {
        enregistrer_texte(nomFichier, matrice, dim);
    }

    liberer_matrice(matrice, dim);
}

if (affichage)
{
    if (binaire)
    {
        afficher_binaire(nomFichier, dim);
    }
    else
    {
        afficher_texte(nomFichier);
    }
}
   

    return 0;
}