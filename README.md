# HeadHunter-API-CPP
hh.ru resume refresh date 

#Getting Started Guide
1. Sign in https://dev.hh.ru/admin 
2. Click Request a token.
3. Open file main.cpp Go to line 17, 18
   * Insert accessToken from step 2
   ```c++
   std::wstring accessToken = L"your accessToken";
   ```
   * Insert resumeId https://spb.hh.ru/applicant/resumes
   ```c++
   std::wstring resumeId = L"your resumeId";
   ```
