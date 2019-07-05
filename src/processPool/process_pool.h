#ifndef PROCESS_POOL
#define PROCESS_POOL
#include <unistd.h>
#include <sys/types.h>

  class process {
    public:
      process(): m_pid(-1){}
    private:
      pid_t m_pid;
      int m_piefd[2];
  };

  template< typename T>
  class processPool{
    private:
      processPool(int listenfd, int process_number = 8);
    public:
      static processPool<T>* create(int listenfd, int process_number = 8) {
        if (!m_instance) {
          m_instance = new processPool<T> (listenfd, process_number);
        }
        return m_instance;
      }
      ~processPool () {
        delete[] m_sub_process;
      }
      void run();
    private:
      void setup_sig_pipe();
      void run_parent();
      void run_child();
    private:
      static const int MAX_PROCESS_NUMBER = 16;
      static const int USER_PER_PROCESS = 65535;
      static const int MAX_EVENT_NUMBER = 10000;
      int m_process_number;
      int m_idx;
      int m_epollfd;
      int m_listenfd;
      int m_stop;
      process * m_sub_process;
      static processPool<T> * m_instance;
  };
#endif
