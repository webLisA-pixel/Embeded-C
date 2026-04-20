struct abonent_list {
	struct abonent_list *prev;
	char name[20];
	char second_name[20];
	char tel[15];
	struct abonent_list *next;
};

struct inform {
	struct abonent_list *first;
	int count;
	struct abonent_list *last;
};

int add_subscriber_first(struct abonent_list *ptr, struct inform *info);
int add_subscriber(struct abonent_list *ptr, struct inform *info);
int delete_subscriber(struct inform *info);
int search_subscriber(struct inform *info);
int print_list(struct inform *info);
int free_mem(struct inform *info);