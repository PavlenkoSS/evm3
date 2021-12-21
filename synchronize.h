
int init_synchronize_lock(int total_processes);
int destroy_synchronize_lock (void);
int lock_data (void);
int unlock_data (void);
void synchronize (int process_num, int total_processes);