#  42Seoul IRC Server

## ✏️ Project Description

[IRC(Internet Relay Chat)](https://en.wikipedia.org/wiki/Internet_Relay_Chat) 서버를 구현하는 프로젝트입니다.

1. 개발 기간 : 2024.01.01 ~ 2024.02.07

## 💻 Features
```
1. IRC 서버 구현: IRC 프로토콜을 기반으로 하는 채팅 서버를 구현합니다.
2. 다중 클라이언트 처리: 멀티플렉싱을 사용하여 동시에 여러 클라이언트의 요청을 처리합니다.
3. 채널 관리: 사용자들이 채널을 생성하고 관리할 수 있습니다.
4. 메시지 전송 및 수신: 클라이언트 간의 실시간 메시징 기능을 구현합니다.
5. 서버 설정: 구성 파일을 통해 서버의 다양한 설정을 관리합니다. (예: 포트 번호, 서버 이름 등)
```

## ⚙️ Stacks
### Development
<img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white"> <img src="https://img.shields.io/badge/Makefile-042E34?style=for-the-badge&logo=gnu-bash&logoColor=white">

### Environment
<img src="https://img.shields.io/badge/github-181717?style=for-the-badge&logo=github&logoColor=white"> <img src="https://img.shields.io/badge/git-F05032?style=for-the-badge&logo=git&logoColor=white"> <img src="https://img.shields.io/badge/visual%20studio%20code-007ACC?style=for-the-badge&logo=visualstudiocode&logoColor=white">
<img src="https://img.shields.io/badge/macOS-000000?style=for-the-badge&logo=macOS&logoColor=white">
<img src="https://img.shields.io/badge/42-000000?style=for-the-badge&logo=42&logoColor=white">

## 🌱 Convention
### Branch Naming Convention
```
   - <lable>/#<issue number>
        |           |           
        |           └─⫸ (Your Issue Number)
        |
        └─⫸ (build|ci|docs|feat|fix|perf|refactor|test)
```
```
< example >
   - feat/#42 - 42번 이슈에 대한 새로운 기능(feature) 추가를 위한 브랜치.
   - bugfix/#17 - 17번 이슈의 버그를 수정하는 작업을 위한 브랜치.
   - docs/#33 - 33번 이슈와 관련된 문서(docs) 업데이트를 위한 브랜치.
```

### Commit Message Convention
   - [Angular Commit Message Format](https://github.com/angular/angular/blob/main/CONTRIBUTING.md#commit)
   ```
   <type>(<scope>): <short summary>
  │       │             │
  │       │             └─⫸ Summary in present tense. Not capitalized. No period at the end.
  │       │
  │       └─⫸ (optional) Commit Scope: animations|bazel|benchpress|common|compiler|compiler-cli|core|
  │                          elements|forms|http|language-service|localize|platform-browser|
  │                          platform-browser-dynamic|platform-server|router|service-worker|
  │                          upgrade|zone.js|packaging|changelog|docs-infra|migrations|
  │                          devtools
  │
  └─⫸ Commit Type: build|ci|docs|feat|fix|perf|refactor|test
   ```
   ```
   < example >
      - feat(feature-name): 기능 구현
      - fix(issue-or-bug): 버그 수정 / 이슈 해결
      - docs(document-name): 프로젝트 문서 업데이트
      - style(style-change): 코드 포맷팅, 세미콜론 누락 등 스타일 관련 변경
      - refactor(refactoring): 코드 리팩토링
      - test(test-name): 테스트 코드 추가 및 수정

   ```
---
### Contributor
   - jonim (@imjongmin123)
   - minkyuki (@mk3058)
   - minseunk (@minseunk42)