#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_BUF 64

int main(int argc, char *argv[]) {
    // 변수 선언
    int fd_origin, fd_dest, read_size, write_size = 0;
    char buf[MAX_BUF];

    // 예외 처리 (사용법 출력)
    if (argc != 3) {
        printf("USAGE: %s origin dest\n", argv[0]);
        exit(-1);
    }

    // [1] 원본 파일 열기 (읽기 전용)
    fd_origin = open(argv[1], O_RDONLY);
    
    // [1-1] 대상 파일 열기 (쓰기 모드, 없으면 생성, 있으면 기존 내용 삭제)
    fd_dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    // 오류 처리
    if (fd_origin < 0 || fd_dest < 0) {
        perror("fd open error");
        exit(-1);
    }

    // [2] 원본 파일에서 읽어서 [3] 대상 파일에 쓰기
    while ((read_size = read(fd_origin, buf, MAX_BUF)) > 0) {
        write_size = write(fd_dest, buf, read_size);

        // if (write_size != read_size) {
        //     perror("write error");
        //     close(fd_origin);
        //     close(fd_dest);
        //     exit(-1);
        // }
    }

    if (read_size < 0) {
        perror("read error");
    }

    // [4] 파일 닫기
    close(fd_origin);
    close(fd_dest);

    return 0;
}
