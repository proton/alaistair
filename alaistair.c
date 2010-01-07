///Alaistair 0.3.0-r3
#include "config.h"

int main()
{
  printf("\nВас приветствует Alaistair! (ver 0.3.0)\n\n");
  log_add("-----------Alaistair started!-----------",40,0);
  printf("[>] Чтение конфигурационных настроек...\n");
  config(address, &port, key, &protocol, login, pwd, &servnum, &charnum);
  printf("[!] Конфигурация успешно считана!\n");
  log_add("Configuration loaded!",21,'a');
  printf("[>] Генерация подключей на основе базового ключа...\n");
  BF_set_key(&bfkey,21,key);
  printf("[!] Генерация подключей успешно завершена!\n");
  cnt();
  //if( m<0 ) recnt();
  return 0;
}

void cnt()
{
  log_add("Connecting to server...",23,'a');
  //la_connect();
  w=0;
//m=-1;
  m = connect_to_ls(&s2,address,port);
  if ( m>=0 ) ls_build_auth(sbuf,login,pwd);
  if ( m>=0 ) m = send_ls_pc(&s2,sbuf,bfkey);
  if ( m>=0 ) m = test_recv_ls(3);
  if ( m>=0 ) ls_build_servlst(sbuf,login_key_p1,login_key_p2);
  if ( m>=0 ) m = send_ls_pc(&s2,sbuf,bfkey);
  if ( m>=0 ) m = test_recv_ls(4);
  if ( m>=0 ) ls_build_servlogin(sbuf,login_key_p1,login_key_p2,servnum);
  if ( m>=0 ) m = send_ls_pc(&s2,sbuf,bfkey);
  if ( m>=0 ) m = test_recv_ls(7);
  if ( m>=0 ) close(s2);
  if ( m>=0 ) m = printf("[!] Соединение с логин-сервером завершено! =D\n");
  if ( m>=0 ) m = connect_to_gs(&s2,servers[servnum-1].ip,servers[servnum-1].port,rbuf,sbuf,key_cs,key_sc,protocol);
  if ( m>=0 ) gs_build_auth(sbuf,login,login_key_p1,login_key_p2,play_key_p1,play_key_p2);
  if ( m>=0 ) m = send_gs_pc(&s2, sbuf,key_cs);
  if ( m>=0 ) m = recv_gs_pc(&s2, rbuf, key_sc);
  if ( m>=0 ) gs_build_charselect(sbuf,charnum);
  if ( m>=0 ) m = send_gs_pc(&s2, sbuf,key_cs);
  if ( m>=0 ) m = recv_gs_pc(&s2, rbuf, key_sc);
  if ( m>=0 ) m = recv_gs_pc(&s2, rbuf, key_sc);
  if ( m>=0 ) gs_build_mlist(sbuf);
  if ( m>=0 ) m = send_gs_pc(&s2, sbuf,key_cs);
  if ( m>=0 ) gs_build_qlist(sbuf);
  if ( m>=0 ) m = send_gs_pc(&s2, sbuf,key_cs);
  if ( m>=0 ) m = recv_gs_pc(&s2, rbuf, key_sc);
  if ( m>=0 ) m = recv_gs_pc(&s2, rbuf, key_sc);
  if ( m>=0 ) gs_build_enterworld(sbuf);
  if ( m>=0 ) m = send_gs_pc(&s2, sbuf,key_cs);
  gameprc();
}

int test_recv_ls(int must_m)
{
  int recv_m = recv_ls_pc(&s2,rbuf,login_key_p1,login_key_p2,play_key_p1,play_key_p2,bfkey,servers);
  if ( recv_m!=must_m && recv_m>=0) test_recv_ls(must_m);
  return recv_m;
}

void recnt()
{
  log_add("Connection with server losed. Waiting 25 sec..",46,'c');
  printf("[X] Соединение с сервером разорвано. Попытка переподключения состоится через\n");
  sleep(1);
  unsigned char isp;
  for (isp=25; isp>0; isp-=5)
    {
      printf(" %i", isp);
      sleep(5);
    }
  cnt();
  sleep(1);
  if (m<0) recnt();
}

void gameprc()
{
  int i=0,l,len=0,size=0;
  init_pl();
  while (m>=0)
    {
star:
      l=recv(s2, rbuf, 65536, 0);
      if (l < 0)
        {
          printf("[X] Ошибка при получении данных\n");
          log_add("Can't get data from GS",22,'c');
          m = -1;
        }
      else if (l>0)
        {
          memcpy(mbuf+len,rbuf,l);
          len+=l;
          if (rbuf[1]*256+rbuf[0]!=l) goto star;
          while (i!=len)
            {
              size=mbuf[i+1]*256+mbuf[i];
              gs_pc_dec(mbuf+2+i,size-2,key_sc);
              game_td(mbuf+i,sbuf,pid);
              i+=size;
            }
          i=len=size=0;
          if (w==0)
            {
              printf("[!] Вход в мир произведен!\n");
              log_add("Entered to world!",17,'w');
              w = 1;
            }
        }
      //if( m==2 ) send_gs_pc(&s2, sbuf,key_cs);
      if (w==1 && gbuf[0]==0x00)
        {
          FILE *file = fopen("ala", "r");
          fseek (file, 0 , SEEK_END);
          size_t fsize = ftell (file);
          rewind (file);
          fread (gbuf, 1, fsize, file);
          fclose(file);
        }
      if (gbuf[0]!=0x00)
        {
          game_recv_cmd(gbuf, "", strlen(gbuf), 0, 0);
          bzero(gbuf, 1000);
          remove("ala");
          FILE *file  = fopen("ala", "w+");
          fclose(file);
        }
    }
}

void gmtd(unsigned char* raw)
{
  switch (raw[0])
    {
    case 0x38://Say
      game_build_say(sbuf,raw+1);
      break;
    case 0x1D://Sit|Stand
      game_build_wtp(sbuf,raw+1);
      break;
    case 0x1C://Go|Run
      game_build_mtp(sbuf,raw+1);
      break;
    case 0x01://Move
      game_build_move(sbuf,raw+1);
      break;
    case 0x1B://Socact
      game_build_socact(sbuf,raw+1);
      break;
    case 0x2F://Use Skill
      game_build_skilluse(sbuf,raw+1);
      break;
    case 0x29://Request Party
      game_build_partyreq(sbuf,raw+1);
      break;
    case 0x2A://Answer JoinParty
      game_build_partyans(sbuf,raw+1);
      break;
    case 0x5E://Request Adding to Friends
      game_build_friendreq(sbuf,raw+1);
      break;
    case 0xA8://Ping
      game_build_ping(sbuf,raw+1);
      break;
    default:
      printf("[!] Невозможно определить тип команды (%d)\n",raw[0]);
      break;
    }
  send_gs_pc(&s2, sbuf,key_cs);
  printf("[>] Команда: #%.2X отпралена на исполнение!\n",raw[0]);
}
