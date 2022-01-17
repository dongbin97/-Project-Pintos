#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "hash.h"
#include "bitmap.h"

#define ARRAY_SIZE 10
#define INDEX_CHECK(idx)		\
	if(idx<0) { printf("ERROR : CANNOT FIND TARGET. YOU ENETERED UNEXISTING NAME.\n"); return; }

/* ====== typedef ====== */
typedef enum {
	/* common commands */
	CREATE, DELETE, DUMPDATA, QUIT, 
	/* list commands */
	LIST_FRONT, LIST_BACK, LIST_INSERT, LIST_INSERT_ORDERED,
	LIST_MAX, LIST_MIN, LIST_SIZE, LIST_EMPTY,
	LIST_PUSH_FRONT, LIST_PUSH_BACK, LIST_POP_FRONT, LIST_POP_BACK,
	LIST_REMOVE, LIST_REVERSE, LIST_SHUFFLE, LIST_SORT,
	LIST_SPLICE, LIST_SWAP, LIST_UNIQUE,
	/* hash commands */
	HASH_INSERT, HASH_DELETE, HASH_APPLY, 
	HASH_SIZE, HASH_EMPTY, HASH_CLEAR, HASH_FIND, HASH_REPLACE,
	/* bitmap commands */
	BITMAP_MARK, BITMAP_ALL, BITMAP_ANY, BITMAP_CONTAINS, BITMAP_COUNT,
	BITMAP_DUMP, BITMAP_EXPAND, BITMAP_FLIP, BITMAP_NONE, BITMAP_RESET,
	BITMAP_SCAN, BITMAP_SCAN_AND_FLIP, BITMAP_SET, BITMAP_SET_ALL, 
	BITMAP_SET_MULTIPLE, BITMAP_SIZE, BITMAP_TEST,
}COMMAND;

typedef enum {
	LIST, HASHTABLE, BITMAP
}DATA_STRUCTURE;

/* ====== global ======*/
struct list_name listArray[ARRAY_SIZE];
struct hash_name hashArray[ARRAY_SIZE];
struct bitmap_name bitmapArray[ARRAY_SIZE];

/* ====== parsing functions ======*/
char* getName() {
	char* name = (char*)malloc(sizeof(char) * 50);
	char* temp = name;
	char ch;
	int idx = 0;

	while ((ch = getchar()) != '\n' && ch != ' ' && ch != EOF)
		*temp++ = ch;
	*temp = '\0';

	return name;
}

COMMAND getCommand() {
	char* command = getName();

	/* common commands */
	if (!strcmp(command, "create")) return CREATE;
	else if (!strcmp(command, "delete")) return DELETE;
	else if (!strcmp(command, "dumpdata")) return DUMPDATA;
	else if (!strcmp(command, "quit")) return QUIT;
	/* list commands */
	else if (!strcmp(command, "list_front")) return LIST_FRONT;
	else if (!strcmp(command, "list_back")) return LIST_BACK;
	else if (!strcmp(command, "list_insert")) return LIST_INSERT;
	else if (!strcmp(command, "list_insert_ordered")) return LIST_INSERT_ORDERED;
	else if (!strcmp(command, "list_max")) return LIST_MAX;
	else if (!strcmp(command, "list_min")) return LIST_MIN;
	else if (!strcmp(command, "list_size")) return LIST_SIZE;
	else if (!strcmp(command, "list_empty")) return LIST_EMPTY;
	else if (!strcmp(command, "list_push_front")) return LIST_PUSH_FRONT;
	else if (!strcmp(command, "list_push_back")) return LIST_PUSH_BACK;
	else if (!strcmp(command, "list_pop_front")) return LIST_POP_FRONT;
	else if (!strcmp(command, "list_pop_back")) return LIST_POP_BACK;
	else if (!strcmp(command, "list_remove")) return LIST_REMOVE;
	else if (!strcmp(command, "list_reverse")) return LIST_REVERSE;
	else if (!strcmp(command, "list_shuffle")) return LIST_SHUFFLE;
	else if (!strcmp(command, "list_sort")) return LIST_SORT;
	else if (!strcmp(command, "list_splice")) return LIST_SPLICE;
	else if (!strcmp(command, "list_swap")) return LIST_SWAP;
	else if (!strcmp(command, "list_unique")) return LIST_UNIQUE;
	/* hash commands */
	else if (!strcmp(command, "hash_insert")) return HASH_INSERT;
	else if (!strcmp(command, "hash_delete")) return HASH_DELETE;
	else if (!strcmp(command, "hash_apply")) return HASH_APPLY;
	else if (!strcmp(command, "hash_size")) return HASH_SIZE;
	else if (!strcmp(command, "hash_empty")) return HASH_EMPTY;
	else if (!strcmp(command, "hash_clear")) return HASH_CLEAR;
	else if (!strcmp(command, "hash_find")) return HASH_FIND;
	else if (!strcmp(command, "hash_replace")) return HASH_REPLACE;
	/* bitmap commands */
	else if (!strcmp(command, "bitmap_mark")) return BITMAP_MARK;
	else if (!strcmp(command, "bitmap_all")) return BITMAP_ALL;
	else if (!strcmp(command, "bitmap_any")) return BITMAP_ANY;
	else if (!strcmp(command, "bitmap_contains")) return BITMAP_CONTAINS;
	else if (!strcmp(command, "bitmap_count")) return BITMAP_COUNT;
	else if (!strcmp(command, "bitmap_dump")) return BITMAP_DUMP;
	else if (!strcmp(command, "bitmap_expand")) return BITMAP_EXPAND;
	else if (!strcmp(command, "bitmap_flip")) return BITMAP_FLIP;
	else if (!strcmp(command, "bitmap_none")) return BITMAP_NONE;
	else if (!strcmp(command, "bitmap_reset")) return BITMAP_RESET;
	else if (!strcmp(command, "bitmap_scan")) return BITMAP_SCAN;
	else if (!strcmp(command, "bitmap_scan_and_flip")) return BITMAP_SCAN_AND_FLIP;
	else if (!strcmp(command, "bitmap_set")) return BITMAP_SET;
	else if (!strcmp(command, "bitmap_set_all")) return BITMAP_SET_ALL;
	else if (!strcmp(command, "bitmap_set_multiple")) return BITMAP_SET_MULTIPLE;
	else if (!strcmp(command, "bitmap_size")) return BITMAP_SIZE;
	else if (!strcmp(command, "bitmap_test")) return BITMAP_TEST;
}

DATA_STRUCTURE getDataStructure() {
	char* data = getName();

	if (!strcmp(data, "list")) return LIST;
	else if (!strcmp(data, "hashtable")) return HASHTABLE;
	else if (!strcmp(data, "bitmap")) return BITMAP;
}

/* ====== preprocess function ====== */
int findEmptyIndex(DATA_STRUCTURE data_struct) {
	int idx = -1;
	switch (data_struct) {
	case LIST:
		for (int i = 0; i < ARRAY_SIZE; i++) {
			if (listArray[i].used == false) {
				idx = i;
				break;
			}
		}
		break;
	case HASHTABLE:
		for (int i = 0; i < ARRAY_SIZE; i++) {
			if (hashArray[i].used == false) {
				idx = i;
				break;
			}
		}
		break;
	case BITMAP:
		for (int i = 0; i < ARRAY_SIZE; i++) {
			if (bitmapArray[i].used == false) {
				idx = i;
				break;
			}
		}
		break;
	}

	return idx;
}

int findTargetIndex(DATA_STRUCTURE data_struct, char* targetName) {
	int idx = -1;

	switch (data_struct) {
	case LIST:
		for (int i = 0; i < ARRAY_SIZE; i++) {
			if (!strcmp(listArray[i].name, targetName)) {
				idx = i;
				break;
			}
		}
		break;
	case HASHTABLE:
		for (int i = 0; i < ARRAY_SIZE; i++) {
			if (!strcmp(hashArray[i].name, targetName)) {
				idx = i;
				break;
			}
		}
		break;
	case BITMAP:
		for (int i = 0; i < ARRAY_SIZE; i++) {
			if (!strcmp(bitmapArray[i].name, targetName)) {
				idx = i;
				break;
			}
		}
		break;
	}
	
	return idx;
}

/* ====== list function ====== */
void createList() { //create new list
	int createPosition = findEmptyIndex(LIST);
	if (createPosition < 0) { //empty index가 없음
		printf("ERROR : ARRAY IS FULL. CANNOT CREATE NEW LIST.\n");
		return;
	}

	if (listArray[createPosition].allocated == false) {
		listArray[createPosition].core = (struct list*)malloc(sizeof(struct list));
		list_init(listArray[createPosition].core);
		listArray[createPosition].allocated = true;
	}

	char* listName = getName();
	strcpy(listArray[createPosition].name, listName);
	
	listArray[createPosition].used = true;
}

void deleteList(int deleteIndex) { //delete list by name
	while (!list_empty(listArray[deleteIndex].core)){
		struct list_elem* e = list_pop_front(listArray[deleteIndex].core);
		list_item_free(e, NULL);
	}

	strcpy(listArray[deleteIndex].name, "\0");
	listArray[deleteIndex].used = false;
}

void dumpdataList(int targetIndex) { //print all datas int list_item
	if (list_empty(listArray[targetIndex].core)) return;

	struct list_elem* it = list_begin(listArray[targetIndex].core);
	struct list_elem* end = list_end(listArray[targetIndex].core);
	struct list_item* item;
	while (it != end) {
		item = list_entry(it, struct list_item, elem);
		printf("%d ", item->data);
		it = list_next(it);
	}
	printf("\n");
}

void list_front_command() { //print front list_item data
	char* targetName = getName(); 
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	if (list_empty(listArray[targetIndex].core)) {
		printf("ERROR : %s IS EMPTY.\n", targetName);
		return;
	}

	struct list_elem* e = list_front(listArray[targetIndex].core);
	struct list_item* item = list_entry(e, struct list_item, elem);
	printf("%d\n", item->data);
}

void list_back_command() { //print back list_item data
	char* targetName = getName();
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	if (list_empty(listArray[targetIndex].core)) {
		printf("ERROR : %s IS EMPTY.\n", targetName);
		return;
	}

	struct list_elem* e = list_back(listArray[targetIndex].core);
	struct list_item* item = list_entry(e, struct list_item, elem);
	printf("%d\n", item->data);
}

void list_insert_command() { //insert new data
	char* targetName = getName();
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	int position; scanf("%d", &position);
	int data; scanf("%d", &data);

	struct list_elem* it = list_begin(listArray[targetIndex].core);
	for (int i = 0; i < position; i++)
		it = list_next(it);

	struct list_elem* e = (struct list_elem*)malloc(sizeof(struct list_elem));
	struct list_item* item = list_entry(e, struct list_item, elem);
	item->data = data;

	list_insert(it, e);
}

void list_insert_ordered_command() { //insert new data and sort list
	char* targetName = getName();
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	int data; scanf("%d", &data);

	struct list_elem* e = (struct list_elem*)malloc(sizeof(struct list_elem));
	struct list_item* item = list_entry(e, struct list_item, elem);
	item->data = data;

	list_insert_ordered(listArray[targetIndex].core, e, list_less, NULL);
}

void list_max_command() { // find max data from list
	char* targetName = getName();
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	struct list_elem* e = list_max(listArray[targetIndex].core, list_less, NULL);
	struct list_item* item = list_entry(e, struct list_item, elem);
	printf("%d\n", item->data);
}

void list_min_command() { // find min data from list
	char* targetName = getName();
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	struct list_elem* e = list_min(listArray[targetIndex].core, list_less, NULL);
	struct list_item* item = list_entry(e, struct list_item, elem);
	printf("%d\n", item->data);
}

void list_size_command() { // find size of list
	char* targetName = getName();
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	printf("%zu\n", list_size(listArray[targetIndex].core));
}

void list_empty_command() { //find list is empty or not
	char* targetName = getName();
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	if (list_empty(listArray[targetIndex].core)) printf("true\n");
	else printf("false\n");
}

void list_push_front_command() { //push element in list front
	char* targetName = getName();
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	int data; scanf("%d", &data);

	struct list_elem* e = (struct list_elem*)malloc(sizeof(struct list_elem));
	struct list_item* item = list_entry(e, struct list_item, elem);
	item->data = data;

	list_push_front(listArray[targetIndex].core, e);
}

void list_push_back_command() { //push element in list back
	char* targetName = getName();
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	int data; scanf("%d", &data);

	struct list_elem* e = (struct list_elem*)malloc(sizeof(struct list_elem));
	struct list_item* item = list_entry(e, struct list_item, elem);
	item->data = data;

	list_push_back(listArray[targetIndex].core, e);
}

void list_pop_front_command() { //pop element from list front
	char* targetName = getName();
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	struct list_elem* e = list_pop_front(listArray[targetIndex].core);
	list_item_free(e, NULL);
}

void list_pop_back_command() { //pop element from list back
	char* targetName = getName();
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	struct list_elem* e = list_pop_back(listArray[targetIndex].core);
	list_item_free(e, NULL);
}

void list_remove_command() { //remove element at certain position from list
	char* targetName = getName();
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	int position; scanf("%d", &position);
	
	struct list_elem* it = list_begin(listArray[targetIndex].core);
	for (int i = 0; i < position; i++)
		it = list_next(it);

	list_remove(it);
	list_item_free(it, NULL);
}

void list_reverse_command() { //reverse elements in list
	char* targetName = getName();
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	list_reverse(listArray[targetIndex].core);
}

void list_shuffle_command() { //shuffle elements in list randomly
	char* targetName = getName();
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	list_shuffle(listArray[targetIndex].core);
}

void list_sort_command() { //sort elements in list
	char* targetName = getName();
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	list_sort(listArray[targetIndex].core, list_less, NULL);
}

void list_splice_command() { //splice some elements from a list to another list
	char* to = getName();
	int toIndex = findTargetIndex(LIST, to);
	INDEX_CHECK(toIndex);

	int insertPosition; scanf("%d ", &insertPosition);
	struct list_elem* dest = list_begin(listArray[toIndex].core);
	for (int i = 0; i < insertPosition; i++) dest = list_next(dest);

	char* from = getName();
	int fromIndex = findTargetIndex(LIST, from);
	INDEX_CHECK(fromIndex);

	int start; scanf("%d ", &start);
	int end; scanf("%d", &end);

	struct list_elem* src_start = list_begin(listArray[fromIndex].core);
	struct list_elem* src_end = list_begin(listArray[fromIndex].core);

	for (int i = 0; i < start; i++) src_start = list_next(src_start);
	for (int i = 0; i < end; i++) src_end = list_next(src_end);

	list_splice(dest, src_start, src_end);
}

void list_swap_command() { //swap two elements in list
	char* targetName = getName();
	int targetIndex = findTargetIndex(LIST, targetName);
	INDEX_CHECK(targetIndex);

	int idx1, idx2; scanf("%d %d", &idx1, &idx2);
	struct list_elem* elem1 = list_begin(listArray[targetIndex].core);
	struct list_elem* elem2 = list_begin(listArray[targetIndex].core);

	for (int i = 0; i < idx1; i++) elem1 = list_next(elem1);
	for (int i = 0; i < idx2; i++) elem2 = list_next(elem2);
	
	list_swap(elem1, elem2);
}

void list_unique_command() { //find unique elements in list
	char first[50], ch;
	int idx = 0;
	bool next = false;

	while (1) {
		ch = getchar();
		if (ch != '\n' && ch != ' ' && ch != EOF) first[idx++] = ch;
		else if (ch == ' ') {
			next = true;
			break;
		}
		else break;
	}

	first[idx] = '\0';
	int firstIndex = findTargetIndex(LIST, first);
	INDEX_CHECK(firstIndex);

	if (next) {
		char* second = getName();
		int secondIndex = findTargetIndex(LIST, second);
		INDEX_CHECK(secondIndex);
		list_unique(listArray[firstIndex].core, listArray[secondIndex].core, list_less, NULL);
	}
	else list_unique(listArray[firstIndex].core, NULL, list_less, NULL);
}

/* ====== hash function ====== */
void createHashtable() { // create new hashtable
	int createPosition = findEmptyIndex(HASHTABLE);
	if (createPosition < 0) { //empty index가 없음
		printf("ERROR : ARRAY IS FULL. CANNOT CREATE NEW HASHTABLE.\n");
		return;
	}

	if (hashArray[createPosition].allocated == false) {
		hashArray[createPosition].core = (struct hash*)malloc(sizeof(struct hash));
		hash_init(hashArray[createPosition].core, hash_func, hash_less, NULL);
		hashArray[createPosition].allocated = true;
	}

	char* hashName = getName();
	strcpy(hashArray[createPosition].name, hashName);

	hashArray[createPosition].used = true;
}

void deleteHashtable(int deleteIndex) { //delete hashtable by name
	hash_clear(hashArray[deleteIndex].core, hash_item_free);
	strcpy(hashArray[deleteIndex].name, "\0");
	hashArray[deleteIndex].used = false;
}

void dumpdataHashTable(int targetIndex) { //print all elements in hashtable
	if (hash_empty(hashArray[targetIndex].core)) return;
	
	struct hash_iterator it;
	hash_first(&it, hashArray[targetIndex].core);

	struct hash_item* item;
	while (hash_next(&it)) {
		item = hash_entry(hash_cur(&it), struct hash_item, elem);
		printf("%d ", item->data);
	}
	printf("\n");
}

void hash_insert_command() { //insert new data in hashtable
	char* targetName = getName();
	int targetIndex = findTargetIndex(HASHTABLE, targetName);
	INDEX_CHECK(targetIndex);

	int data; scanf("%d", &data);

	struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
	struct hash_item* item = hash_entry(e, struct hash_item, elem);
	item->data = data;

	hash_insert(hashArray[targetIndex].core, e);
}

void hash_delete_command() { //delete data from a hashtable
	char* targetName = getName();
	int targetIndex = findTargetIndex(HASHTABLE, targetName);
	INDEX_CHECK(targetIndex);

	int data; scanf("%d", &data);

	struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
	struct hash_item* item = hash_entry(e, struct hash_item, elem);
	item->data = data;

	struct hash_elem* del = hash_delete(hashArray[targetIndex].core, e);
	if(del != NULL) hash_item_free(del, NULL);
}

void hash_apply_command() { //apply given function to all elements in hashtable
	char* targetName = getName();
	int targetIndex = findTargetIndex(HASHTABLE, targetName);
	INDEX_CHECK(targetIndex);

	char* option = getName();
	if (!strcmp(option, "square")) hash_apply(hashArray[targetIndex].core, hash_square);
	else if (!strcmp(option, "triple")) hash_apply(hashArray[targetIndex].core, hash_triple);
	else printf("ERROR : INVALID OPTION.\n");
}

void hash_empty_command() { //find hashtable is empty or not
	char* targetName = getName();
	int targetIndex = findTargetIndex(HASHTABLE, targetName);
	INDEX_CHECK(targetIndex);

	if (hash_empty(hashArray[targetIndex].core)) printf("true\n");
	else printf("false\n");
}

void hash_size_command() { // find size of hashtable
	char* targetName = getName();
	int targetIndex = findTargetIndex(HASHTABLE, targetName);
	INDEX_CHECK(targetIndex);

	printf("%zu\n", hash_size(hashArray[targetIndex].core));
}

void hash_clear_command() { //clear all elements in hashtable
	char* targetName = getName();
	int targetIndex = findTargetIndex(HASHTABLE, targetName);
	INDEX_CHECK(targetIndex);

	hash_clear(hashArray[targetIndex].core, hash_item_free);
}

void hash_find_command() { //find certain element in hashtable
	char* targetName = getName();
	int targetIndex = findTargetIndex(HASHTABLE, targetName);
	INDEX_CHECK(targetIndex);

	int data; scanf("%d", &data);

	struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
	struct hash_item* item = hash_entry(e, struct hash_item, elem);
	item->data = data;

	struct hash_elem* res = hash_find(hashArray[targetIndex].core, e);
	if (res != NULL) {
		struct hash_item* res_item = hash_entry(res, struct hash_item, elem);
		printf("%d\n", res_item->data);
	}
}

void hash_replace_command() { //replace elements in hashtable with a elememt that has same data
	char* targetName = getName();
	int targetIndex = findTargetIndex(HASHTABLE, targetName);
	INDEX_CHECK(targetIndex);

	int data; scanf("%d", &data);

	struct hash_elem* e = (struct hash_elem*)malloc(sizeof(struct hash_elem));
	struct hash_item* item = hash_entry(e, struct hash_item, elem);
	item->data = data;

	hash_replace(hashArray[targetIndex].core, e);
}

/* ====== bitmap function ====== */
void createBitmap() { //create new bitmap
	int createPosition = findEmptyIndex(BITMAP);
	if (createPosition < 0) { //empty index가 없음
		printf("ERROR : ARRAY IS FULL. CANNOT CREATE NEW BITMAP.\n");
		return;
	}

	char* bitmapName = getName();
	strcpy(bitmapArray[createPosition].name, bitmapName);
	
	int bit_cnt; scanf("%d", &bit_cnt);

	bitmapArray[createPosition].core = bitmap_create(bit_cnt);
	bitmapArray[createPosition].used = true;
}

void deleteBitmap(int deleteIndex) { //delete bitmap
	bitmap_destroy(bitmapArray[deleteIndex].core);
	bitmapArray[deleteIndex].used = false;
}

void dumpdataBitmap(int targetIndex) { //print bitmap
	size_t size = bitmap_size(bitmapArray[targetIndex].core);

	for (size_t i = 0; i < size; i++)
		printf("%d", bitmap_test(bitmapArray[targetIndex].core, i));
	printf("\n");
}

void bitmap_mark_command() { //mark certain index in bitmap
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	int idx; scanf("%d", &idx);

	bitmap_mark(bitmapArray[targetIndex].core, idx);
}

void bitmap_all_command() { //find if all bits in given range of bitmap are 1
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	int start; scanf("%d", &start);
	int cnt; scanf("%d", &cnt);

	if (bitmap_all(bitmapArray[targetIndex].core, start, cnt)) printf("true\n");
	else printf("false\n");
}

void bitmap_any_command() { //find if any one bit in given range of bitmap is 1
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	int start; scanf("%d", &start);
	int cnt; scanf("%d", &cnt);

	if (bitmap_any(bitmapArray[targetIndex].core, start, cnt)) printf("true\n");
	else printf("false\n");
}

void bitmap_contains_command() { //find if any one bit in given range of bitmap is given truth value
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	int start; scanf("%d", &start);
	int cnt; scanf("%d ", &cnt);
	bool value;

	char* flag = getName();
	if (!strcmp(flag, "true")) value = true;
	else value = false;

	if (bitmap_contains(bitmapArray[targetIndex].core, start, cnt, value)) printf("true\n");
	else printf("false\n");
}

void bitmap_count_command() { //find the number of bits which has given truth value in given range of bitmap
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	int start; scanf("%d", &start);
	int cnt; scanf("%d ", &cnt);
	bool value;

	char* flag = getName();
	if (!strcmp(flag, "true")) value = true;
	else value = false;

	printf("%zu\n", bitmap_count(bitmapArray[targetIndex].core, start, cnt, value));
}

void bitmap_dump_command() { //represent data in bitmap as hexadecimal
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	bitmap_dump(bitmapArray[targetIndex].core);
}

void bitmap_expand_command() { //expand size of bitmap and fill 0
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	int size; scanf("%d", &size);

	bitmapArray[targetIndex].core = bitmap_expand(bitmapArray[targetIndex].core, size);
}

void bitmap_flip_command() { //flip certain bit in bitmap
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	int idx; scanf("%d", &idx);

	bitmap_flip(bitmapArray[targetIndex].core, idx);
}

void bitmap_none_command() { //find if all bits in given range of bitmap does not have 0
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	int start; scanf("%d", &start);
	int cnt; scanf("%d", &cnt);

	if (bitmap_none(bitmapArray[targetIndex].core, start, cnt)) printf("true\n");
	else printf("false\n");
}

void bitmap_reset_command() { //set certain index of bitmap as 0
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	int idx; scanf("%d", &idx);

	bitmap_reset(bitmapArray[targetIndex].core, idx);
}

void bitmap_scan_command() { //find first index of bit which has same truth value as given truth value in given range of bitmap
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	int start; scanf("%d", &start);
	int cnt; scanf("%d ", &cnt);
	bool value;

	char* flag = getName();
	if (!strcmp(flag, "true")) value = true;
	else value = false;

	printf("%zu\n", bitmap_scan(bitmapArray[targetIndex].core, start, cnt, value));
}

void bitmap_scan_and_flip_command() {  //find first index of bit which has same truth value as given truth value  and flip all bits in given range of bitmap
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	int start; scanf("%d", &start);
	int cnt; scanf("%d ", &cnt);
	bool value;

	char* flag = getName();
	if (!strcmp(flag, "true")) value = true;
	else value = false;

	printf("%zu\n", bitmap_scan_and_flip(bitmapArray[targetIndex].core, start, cnt, value));
}

void bitmap_set_command() { //set certain bit in bitmap with given truth value
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	int idx; scanf("%d ", &idx);
	bool value;

	char* flag = getName();
	if (!strcmp(flag, "true")) value = true;
	else value = false;

	bitmap_set(bitmapArray[targetIndex].core, idx, value);
}

void bitmap_set_all_command() { //set all bits in bitmap with given truth value
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	bool value;

	char* flag = getName();
	if (!strcmp(flag, "true")) value = true;
	else value = false;

	bitmap_set_all(bitmapArray[targetIndex].core, value);
}

void bitmap_set_multiple_command() { //set all bits in given range of bitmap with given truth value
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	int start; scanf("%d", &start);
	int cnt; scanf("%d ", &cnt);
	bool value;

	char* flag = getName();
	if (!strcmp(flag, "true")) value = true;
	else value = false;

	bitmap_set_multiple(bitmapArray[targetIndex].core, start, cnt, value);
}

void bitmap_size_command() { //find size of bitmap
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	printf("%zu\n", bitmap_size(bitmapArray[targetIndex].core));
}

void bitmap_test_command() { //find truth value of certain bit in bitmap
	char* targetName = getName();
	int targetIndex = findTargetIndex(BITMAP, targetName);
	INDEX_CHECK(targetIndex);

	int idx; scanf("%d", &idx);

	if (bitmap_test(bitmapArray[targetIndex].core, idx)) printf("true\n");
	else printf("false\n");
}

/* ====== init function ====== */
void init() {
	for (int i = 0; i < ARRAY_SIZE; i++) {
		listArray[i].allocated = false;
		listArray[i].used = false;

		hashArray[i].allocated = false;
		hashArray[i].used = false;

		bitmapArray[i].used = false;
	}
}

/* ====== common functions ======*/
void create() {
	DATA_STRUCTURE data_struct = getDataStructure();

	switch (data_struct) {
	case LIST:
		createList();
		break;
	case HASHTABLE:
		createHashtable();
		break;
	case BITMAP:
		createBitmap();
		break;
	}
}

void delete() {
	char* deleteName = getName();
	int deleteIndex;

	if ((deleteIndex = findTargetIndex(LIST, deleteName)) >= 0) {
		deleteList(deleteIndex);
		return;
	}
	else if ((deleteIndex = findTargetIndex(HASHTABLE, deleteName)) >= 0) {
		deleteHashtable(deleteIndex);
		return;
	}
	else if ((deleteIndex = findTargetIndex(BITMAP, deleteName)) >= 0) {
		deleteBitmap(deleteIndex);
		return;
	}

	printf("ERROR : CANNOT FIND TARGET. YOU ENETERED UNEXISTING NAME.\n");
	return;
}

void dumpdata() {
	char* targetName = getName(); 
	int targetIndex;

	if ((targetIndex = findTargetIndex(LIST, targetName)) >= 0) {
		dumpdataList(targetIndex);
		return;
	}
	else if ((targetIndex = findTargetIndex(HASHTABLE, targetName)) >= 0) {
		dumpdataHashTable(targetIndex);
		return;
	}
	else if ((targetIndex = findTargetIndex(BITMAP, targetName)) >= 0) {
		dumpdataBitmap(targetIndex);
		return;
	}

	printf("ERROR : CANNOT FIND TARGET. YOU ENETERED UNEXISTING NAME.\n");
	return;
}

void quit() {
	for (int i = 0; i < ARRAY_SIZE; i++) {
		if (listArray[i].allocated == true) {
			deleteList(i);
			free(listArray[i].core);
			listArray[i].allocated = false;
		}

		if (hashArray[i].allocated == true) {
			deleteHashtable(i);
			free(hashArray[i].core);
			hashArray[i].allocated = false;
		}

		if (bitmapArray[i].used == true) 
			deleteBitmap(i);
	}
}

/* ====== main function ====== */
int main() {

	init();

	while (1) {
		COMMAND command = getCommand();
		
		if (command == QUIT) {
			quit();
			break;
		}

		switch (command) {
		/* common commands */
		case CREATE:
			create();
			break;
		case DELETE:
			delete();
			break;
		case DUMPDATA:
			dumpdata();
			break;
		/* list commands */
		case LIST_FRONT:
			list_front_command();
			break;
		case LIST_BACK:
			list_back_command();
			break;
		case LIST_INSERT:
			list_insert_command();
			break;
		case LIST_INSERT_ORDERED:
			list_insert_ordered_command();
			break;
		case LIST_MAX:
			list_max_command();
			break;
		case LIST_MIN:
			list_min_command();
			break;
		case LIST_SIZE:
			list_size_command();
			break;
		case LIST_EMPTY:
			list_empty_command();
			break;
		case LIST_PUSH_FRONT:
			list_push_front_command();
			break;
		case LIST_PUSH_BACK:
			list_push_back_command();
			break;
		case LIST_POP_FRONT:
			list_pop_front_command();
			break;
		case LIST_POP_BACK:
			list_pop_back_command();
			break;
		case LIST_REMOVE:
			list_remove_command();
			break;
		case LIST_REVERSE:
			list_reverse_command();
			break;
		case LIST_SHUFFLE:
			list_shuffle_command();
			break;
		case LIST_SORT:
			list_sort_command();
			break;
		case LIST_SPLICE:
			list_splice_command();
			break;
		case LIST_SWAP:
			list_swap_command();
			break;
		case LIST_UNIQUE:
			list_unique_command();
			break;
		/* hash commands */
		case HASH_INSERT:
			hash_insert_command();
			break;
		case HASH_DELETE:
			hash_delete_command();
			break;
		case HASH_APPLY:
			hash_apply_command();
			break;
		case HASH_SIZE:
			hash_size_command();
			break;
		case HASH_EMPTY:
			hash_empty_command();
			break;
		case HASH_CLEAR:
			hash_clear_command();
			break;
		case HASH_FIND:
			hash_find_command();
			break;
		case HASH_REPLACE:
			hash_replace_command();
			break;
		/* bitmap commands */
		case BITMAP_MARK:
			bitmap_mark_command();
			break;
		case BITMAP_ALL:
			bitmap_all_command();
			break;
		case BITMAP_ANY:
			bitmap_any_command();
			break;
		case BITMAP_CONTAINS:
			bitmap_contains_command();
			break;
		case BITMAP_COUNT:
			bitmap_count_command();
			break;
		case BITMAP_DUMP:
			bitmap_dump_command();
			break;
		case BITMAP_EXPAND:
			bitmap_expand_command();
			break;
		case BITMAP_FLIP:
			bitmap_flip_command();
			break;
		case BITMAP_NONE:
			bitmap_none_command();
			break;
		case BITMAP_RESET:
			bitmap_reset_command();
			break;
		case BITMAP_SCAN:
			bitmap_scan_command();
			break;
		case BITMAP_SCAN_AND_FLIP:
			bitmap_scan_and_flip_command();
			break;
		case BITMAP_SET:
			bitmap_set_command();
			break;
		case BITMAP_SET_ALL:
			bitmap_set_all_command();
			break;
		case BITMAP_SET_MULTIPLE:
			bitmap_set_multiple_command();
			break;
		case BITMAP_SIZE:
			bitmap_size_command();
			break;
		case BITMAP_TEST:
			bitmap_test_command();
			break;
		}
	}
}
