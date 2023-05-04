extern void _transpose(int* target, int* destination, int r, int c, int tid);
extern void transpose(int* target, int* destination, int r, int c);
extern std::vector<std::thread> threads;

void tA0_B0(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = 0 + tid*(r/2);
  int end_row = r/2 + tid*(r/2);
  
  for(int i = start_row; i < end_row; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[i*r*2 + j] += mat1[i*r*2 + k] * mat2[j*c*2 + k]; // no shifting
}

void tA0_B1(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = 0 + tid*(r/2);
  int end_row = r/2 + tid*(r/2);
  
  for(int i = start_row; i < end_row; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[i*r*2 + j+c] += mat1[i*r*2 + k] * mat2[(j+r)*c*2 + k]; // keep A, row B
}

void tA1_B2(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = 0 + tid*(r/2);
  int end_row = r/2 + tid*(r/2);
  
  for(int i = start_row; i < end_row; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[i*r*2 + j] += mat1[i*r*2 + k+c] * mat2[j*c*2 + k+c]; // col A, col B
}

void tA1_B3(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = 0 + tid*(r/2);
  int end_row = r/2 + tid*(r/2);
  
  for(int i = start_row; i < end_row; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[i*r*2 + j+c] += mat1[i*r*2 + k+c] * mat2[(j+r)*c*2 + k+c]; // col A, rowB col B
}

void tA2_B0(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = 0 + tid*(r/2);
  int end_row = r/2 + tid*(r/2);
  
  for(int i = start_row; i < end_row; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[(i+r)*r*2 + j] += mat1[(i+r)*r*2 + k] * mat2[j*c*2 + k]; // row A
}

void tA2_B1(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = 0 + tid*(r/2);
  int end_row = r/2 + tid*(r/2);
  
  for(int i = start_row; i < end_row; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[(i+r)*r*2 + j+c] += mat1[(i+r)*r*2 + k] * mat2[(j+r)*c*2 + k]; // row A, row B
}

void tA3_B2(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = 0 + tid*(r/2);
  int end_row = r/2 + tid*(r/2);
  
  for(int i = start_row; i < end_row; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[(i+r)*r*2 + j] += mat1[(i+r)*r*2 + k+c] * mat2[j*c*2 + k+c]; // col row A, col B
}

void tA3_B3(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = 0 + tid*(r/2);
  int end_row = r/2 + tid*(r/2);
  
  for(int i = start_row; i < end_row; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[(i+r)*r*2 + j+c] += mat1[(i+r)*r*2 + k+c] * mat2[(j+r)*c*2 + k+c]; // col row A, col row B
}

void proc0(int* mat1, int* mat2, int* mat3, int r, int c){
  std::thread t0(tA0_B0, mat1,mat2,mat3,r,c,0);
  std::thread t1(tA0_B1, mat1,mat2,mat3,r,c,0);
  std::thread t2(tA2_B0, mat1,mat2,mat3,r,c,0);
  std::thread t3(tA2_B1, mat1,mat2,mat3,r,c,0);
  std::thread t4(tA1_B2, mat1,mat2,mat3,r,c,1);
  std::thread t5(tA1_B3, mat1,mat2,mat3,r,c,1);
  std::thread t6(tA3_B2, mat1,mat2,mat3,r,c,1);
  std::thread t7(tA3_B3, mat1,mat2,mat3,r,c,1);
  
  t0.join();
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();
  t7.join();
}

void proc1(int* mat1, int* mat2, int* mat3, int r, int c){
  std::thread t0(tA0_B0, mat1,mat2,mat3,r,c,1);
  std::thread t1(tA0_B1, mat1,mat2,mat3,r,c,1);
  std::thread t2(tA2_B0, mat1,mat2,mat3,r,c,1);
  std::thread t3(tA2_B1, mat1,mat2,mat3,r,c,1);
  std::thread t4(tA1_B2, mat1,mat2,mat3,r,c,0);
  std::thread t5(tA1_B3, mat1,mat2,mat3,r,c,0);
  std::thread t6(tA3_B2, mat1,mat2,mat3,r,c,0);
  std::thread t7(tA3_B3, mat1,mat2,mat3,r,c,0);
  
  t0.join();
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();
  t7.join();
}

void blocked_transpose(int* mat1, int* mat2, int* mat3, int r, int c){
  std::chrono::duration<double> diff;
  auto start_time = std::chrono::steady_clock::now();
  int r2 = r/2;
  int c2 = c/2;
  int* transposed = generate_0(r,c);
  transpose(mat2, transposed,r,c);

  proc0(mat1, transposed, mat3, r2,c2);
  proc1(mat1, transposed, mat3, r2,c2);
  
  auto end_time = std::chrono::steady_clock::now();
  diff = end_time - start_time;
  std::cout << "B Transpose Matrix took: " << diff.count() << " sec" << std::endl;
}
