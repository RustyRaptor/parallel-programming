pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t nonempty = PTHREAD_COND_INITIALIZER;
pthread_cond_t nonfull = PTHREAD_COND_INITIALIZER;

int buffer[SIZE];

int put = 0;
int get = 0;

void insert (int x)
{
        pthread_mutex_lock (&lock);
        while (
                (put > get && (put - get) == SIZE - 1) ||
                (put < get && (get + put) == SIZE - 1)
        ) {
                pthread_cond_wait (&nonfull, &lock);
        }
        buffer[put] = x;
        put = (put + 1) % SIZE;
        pthread_cond_signal (&nonempty);
        pthread_mutex_unlock (&lock);

}

int remove ()
{
        int x;
        pthread_mutex_lock (&lock);
        while (put == get) {
                pthread_cond_wait (&nonempty, &lock);
        }
        x = buffer[get];
        get = (get + 1) % SIZE;
        pthread_cond_signal (&nonfull);
        pthread_mutex_unlock (&lock);
        return x;
        
}