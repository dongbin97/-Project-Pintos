# Pintos
Operating System - Linux기반 Pintos 프로젝트

## Project 0_2
Pintos project에서 사용하는 자료구조인 list, hash table, bitmap에 대한 구조를 익히고 각 자료구조를 활용하기 위한 함수를 구현하는 사전 연습 프로젝트

<br>

## Project 1
 ### 개발 목표 
  - process의 기본적인 halt, execute, wait, exit system call을 구현
  - stdin, stdout에 대한 read, write system call을 구현
 <br>       
 
 ### 개발 과정
  1. `argument passing`: 프로그램 수행 명령을 blank기준으로 parsing하고 kernel 내 stack에 argument를 쌓음
  2. `user memory access`: user program이 kernel memory 영역을 침범하는 경우 exit하도록 구현
  3. `system call`: halt, execute, wait, exit, read, write system call 구현
<br>

### 주의 사항
 - <b>exit() system call 구현 시</b>   
 thread_exit()함수를 호출하는데, thread_exit()함수는 process_exit()함수를 호출한다.  
 이때 process_exit() 함수가 호출 되기 전 synchronization이 필요하다.  
 왜냐하면 현재 exit 하고자하는 thread의 부모 thread의 child_list에서 현재 thread가 제거되기 전에 현재 thread가 먼저 exit되어 메모리가 해제되면 안되기 때문이다.  
 
 - <b>wait() system call 구현 시</b>  
 wait()의 역할은 부모 thread 입장에서 특정 child thread가 본인의 일을 마무리하고 exit하기까지를 기다려 주는 일  
 기다리는 이유는 child thread가 본인의 일을 마치고 exit()를 호출하면 부모 thread 입장에서는 child_list에서 해당 child thread를 제거해야 하기 떄문  
 
> `project 1에서 가장 중요한 flow`는  
> 부모 thread의 wait()호출로 자식 thread가 exit하기를 기다리는 중  
> -> 자식 thread에서 exit()를 호출하면 flag를 통해 부모 thread에 해당 자식 thread가 업무를 마쳤음을 알림  
> -> 부모 thread는 child_list에서 해당 자식 thread를 지우고 child_list에서 지워졌다는 사실을 자식 thread에게 알림 (부모 thread의 wait() 종료)  
> -> 자식 thread는 이제야 메모리를 해제하고 실제 exit() 과정이 수행됨 (자식 thread의 exit() 종료)

<br>

## Project 2
### 개발 목표
 - file과 관련된 open, read, write system call을 구현
 - project 1에서는 stdin, stdout에 대한 read, write system call만 구현하였는데 이를 전체 file에 대해 확장
 <br>
 
 ### 개발 과정
 1. `file descriptor`: thread가 open하는 file마다 file descriptor를 각각 지정
 2. `system call`: create, remove, open, filesize, read, write, seek, tell, close system call을 구현
 3. `synchronization in file system`: 현재 thread가 특정 file을 write하는 경우 또는 실행중인 thread의 경우 다른 thread가 해당 file에 read, write하지 못하도록 synchronization이 필요 

### 주의 사항
 - <b>open system call 구현 시</b>  
 open system call은 새로운 파일을 open하기 위함
 현재 thread는 새로 open된 file에 대한 file descriptor를 지정해 주어야함 
 해당 구현 과정에서 sequential search를 사용해 사용하지 않는 file descriptor를 찾아주었지만 circular하게 사용하지 않는 file descriptor를 찾아주는 방법도 있을 것  
 현재 실행 중인 file은 write가 이루어지면 안되므로 thread에 current_file이라는 필드를 추가하여 현재 실행 thread의 file이름을 저장해 놓아야 함
 
 - <b> write system call 수정 사항</b>  
 write하고자 하는 file의 이름과 현재 thread의 current_file이름이 같으면 file_deny_write()함수를 호출해 write가 일어나지 못하도록 한다.
 
 - <b> exit system call 수정 사항 </b>  
 현재 thread가 exit될 때 thread에서 open 해놓은 file을 모두 close 해주어야 함   
thread가 종료될 때 open되어 있는 file이 있으면 안됨, file을 모두 close해야함
write할 때 현재 write하고자 하는 파일이 실행중인 파일인지 확인

 - <b>syncheonization 구현</b>  
 현재 시스템에서는 open, read, write의 동작이 일어나는 경우를 모두 semaphore를 이용해 하나의 thread만 해당 file에 접근할 수 있도록 구현해주었음
 하지만 read의 경우에는 여러 thread가 동시에 수행할 수 있으므로 reader - writer 방식으로 구현해볼 수도 있을 것 (시도해봤으나 테스트를 통과하지 못함)
 
 <br>

## Project 3
### 개발 목표
 - busy waiting으로 동작하는 timer_sleep()함수는 RUN 상태와 READY 상태를 반복적으로 돌기 때문에 비효울적이므로 이를 BLOCK상태로 만들고 sleep_list에 넣어줌
 - priority scheduler를 구현
 - nice, recent_cpu, load_avg등의 값을 이용해 ready_list의 priority를 지속적으러 update해 효율적인 scheduling이 이루어지도록 함
 <br>
 
 ### 개발 과정
 1. `alarm clock`: busy waiting 방식의 timer_sleep()함수는 CPU를 낭비하므로 sleep하는 thread들을 sleep_list에 넣어두고 깨어나야할 시간에 깨워줌 
 2. `priority scheduler`: 단순히 round robin 방식으로 동작하는 scheduler를 priority scheduler로 바꾸어 줌
 3. `advanced scheduler`: thread의 priority를 nice, recent_cpu, load_avg를 이용해 계속 update하여 starvation을 막아줌

> <b>참고 사항</b>  
>  - nice: thread가 다른 thread에 영향을 미치는 정도 (-20 ~ 20)    
>  nice > 0 인 경우에는 자신의 priority를 낮춰 CPU를 다른 thread에 양보  
>  nice < 0인 경우에는 자신의 priority를 높여 CPU를 더 많이 차지  
>  nice가 0인 경우에는 다른 thread에 영향을 미치지 않음
>
> - recent_cpu: 현재 thread의 다음 CPU time을 exponential weighted moving average방법을 이용해 예측한 값
> recent_cpu가 작을수록 높은 priority를 가지게 됨
>
> - load_avg: thread가 아닌 시스템 전체에 귀속된 값으로 최근 1분 동안 수행가능한 thread의 평균 개수
> recent_cpu값 계산에 이용됨

<br>

### 주의 사항
 
 - timer_interrupt마다 sleep_list에서 깨어나야 할 thread를 thread_unblock()함수로 깨워줌  
 - <b>priority scheduler 구현 시</b>  
 linked list구조인 ready_list가 priority queue처럼 동작하게 하기 위해서는 ready_list에 thread를 추가하는 경우에 list_insert_ordered()함수를 이용함으로써 ready_list가 항상 thread의 priority의 내림차순으로 정렬되도록 한다  
 (ready_list를 정렬상태로 유지하지 않고 scheduling할 때 정렬하여 priority가 가장 높은 thread를 꺼내주는 방법도 고려해봤지만 thread의 priority가 update되는 경우 preemtive하게 scheduling되어야하므로 ready_list가 정렬된 상태를 유지하는 것이 효율적이라 판단했음)
 
 - <b> semaphore 수정 사항 </b>  
 ready_list를 thread priority의 내림차순으로 정렬된 상태를 유지하는 것 처럼 sema_waiters내의 thread도 priority의 내림차순으로 정렬된 상태를 유지한다
 
 
