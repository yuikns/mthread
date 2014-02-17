#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <vector>
#include <map>

using namespace std;


typedef struct
{
	string * __dir;
	int id;
	map <int,char> * __kv;
	bool flag ; // success or failed
}idx_item;

void * store_thread(void * __item);
void * load_thread(void * __item);


void * store_thread(void * __item)
{
	idx_item * __info = (idx_item * ) __item;
	string file_name(*__info->__dir);
	file_name += to_string(__info->id);
	fprintf(stdout ,"storing %s...\n",file_name.c_str());
	FILE * fp = fopen( file_name.c_str(),"wb");
	if(fp == NULL )
	{
		__info->flag = false;
		return NULL; 
	}
	int kv_size =  __info->__kv->size();
	fwrite(&kv_size,sizeof(int),1,fp);
	printf("[W] size : %d\n",kv_size);
	for(map<int,char>::iterator iter = __info->__kv->begin(); iter!= __info->__kv->end(); ++iter)
	{
		fwrite(&(iter->first),sizeof(int),1,fp);
		fwrite(&(iter->second),sizeof(char),1,fp);
		fprintf(stdout,"[W] k : %d  v : %c \n",iter->first,iter->second);
	}
	fclose(fp);
	__info->flag = true;
	return NULL;
}

void * load_thread(void * __item)
{
	idx_item * __info = (idx_item * ) __item;
	string file_name(*__info->__dir);
	file_name += to_string(__info->id);
	fprintf(stdout ,"loading %s...\n",file_name.c_str());
	FILE * fp = fopen( file_name.c_str(),"rb");
	if(fp == NULL )
	{
		__info->flag = false;
		return NULL; 
	}
	int kv_size = 0;
	fread(&kv_size,sizeof(int),1,fp);
	printf("[R] size : %d\n",kv_size);
	int k;
	char v;
	for(int i = 0 ; i < kv_size ; i ++ )
	{
		fread(&k,sizeof(int),1,fp);
		fread(&v,sizeof(char),1,fp);
		__info->__kv->insert (std::pair<int,char>(k,v));
		fprintf(stdout,"[R] k : %d  v : %c \n",k,v);
	}
	fclose(fp);
	__info->flag = true;
	return NULL;
}

void show(const char * h , map<int,char> & info )
{
	for(map<int,char>::iterator iter = info.begin(); iter!= info.end(); ++iter)
	{
		printf("[%s] %d - %c \n",h,iter->first,iter->second);
	}
}

int main(int argc, char *argv[])
{
	int map_size = 3;
	map<int,char> smap[map_size];
	map<int,char> dmap[map_size];
	smap[0].insert (std::pair<int,char>(8,'a'));
	smap[0].insert (std::pair<int,char>(7,'b'));
	smap[1].insert (std::pair<int,char>(6,'c'));
	smap[1].insert (std::pair<int,char>(5,'d'));
	smap[2].insert (std::pair<int,char>(4,'e'));
	smap[2].insert (std::pair<int,char>(3,'f'));
	
	
	pthread_t  * pt = new pthread_t[map_size];
	
	string dir("pf_");
	
	fprintf(stdout,"store into file\n");
	idx_item * item = new idx_item[map_size];
	for(int i = 0 ; i < map_size ; i ++ )
	{
		
		item[i].__dir = &dir;
		item[i].id = i;
		item[i].__kv = & smap[i];
		item[i].flag = false;
		int ret = pthread_create(&pt[i],NULL,store_thread,&item[i]);
		if(ret!=0) fprintf(stderr,"[%d] error created \n",i);
	}
	for(int i = 0 ; i < map_size ; i ++ )
	{
		pthread_join(pt[i],NULL);
	}
	delete item;
	delete pt;
	fprintf(stdout,"all stored \nshow each source data : \n");
	for(int i = 0 ; i < map_size ; i ++)
	{
		
		show((string ("src") + to_string(i)).c_str(),smap[i]);
	}
	fprintf(stdout,"###################################\n");
	pt = new pthread_t[map_size];
	item = new idx_item[map_size];
	for(int i = 0 ; i < map_size ; i ++ )
	{
		item[i].__dir = &dir;
		item[i].id = i;
		item[i].__kv = & dmap[i];
		item[i].flag = false;
		int ret = pthread_create(&pt[i],NULL,load_thread,&item[i]);
		if(ret!=0) fprintf(stderr,"[%d] error created \n",i);
	}
	
	
	for(int i = 0 ; i < map_size ; i ++ )
	{
		pthread_join(pt[i],NULL);
	}
	delete item;
	delete pt;
	for(int i = 0 ; i < map_size ; i ++)
	{
		show((string ("dst") + to_string(i)).c_str(),dmap[i]);
	}

	
	
	return 0;
}
