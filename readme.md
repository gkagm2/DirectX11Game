History video
[https://www.youtube.com/watch?v=KTVVbvkEQa0&list=PL-Ka9bftppLxXEridSRBd848nGoytvqng&index=1]

+ x64버전만 사용한다.

+ MBCS, WBCS 동시 지원하게 만듬
++ 폴더 소개
OutputFile - Release, Debug  실행파일 모음
External - 외부 종속 파일
 header는 Include 폴더 내에
 library는 Library 폴더 내에 넣어준다.

Engine Library는같이 개발하므로 Engine에서 빌드 시 External쪽에서 생성됨.
Client는 External쪽에 있는 파일들을 참조하여 새 파일을 만들어 낸다.



