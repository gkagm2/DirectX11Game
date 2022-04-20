History video
[https://www.youtube.com/watch?v=KTVVbvkEQa0&list=PL-Ka9bftppLxXEridSRBd848nGoytvqng&index=1]

+ x64버전만 사용한다.

+ MBCS, WBCS 동시 지원하게 만듬
++ 폴더 소개
OutputFile - Release, Debug  실행파일 모음
External - 외부 종속 파일
 header는 Include 폴더 내에
 library는 Library 폴더 내에 넣어준다.


Engine 프로젝트에서 빌드 시 External 폴더 안에 생성됨. 
Client 프로젝트는 External쪽에 있는 파일들을 참조하여 새 파일을 만들어 낸다.

# 게임 실행
My Game 폴더 안에 실행 파일이 있음.


# 프로젝트 실행 방법.

step 1. 
Download

step 2. 파일 다운로드
External\Library\FBXLoader.zip파일 압축 해제

step 3. 프로젝트 설정 변경

## Engine, Client 프로젝트 속성에서 C/C++ -> 추가 포함 디렉터리에 boost 라이브러리를 추가

## Engine, Client 프로젝트 속성에서 디버깅 -> 작업 디렉터리에 $(SolutionDir)\OutputFile\bin\ 로 경로 바꾸기.
