extern void _transpose(int* target, int* destination, int r, int c, int tid);
extern void transpose(int* target, int* destination, int r, int c);
extern std::vector<std::thread> threads;

void tA0_B0(int* mat1, int* mat2, int* mat3, int r, int c){
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[i*r*2 + j] += mat1[i*r*2 + k] * mat2[j*c*2 + k]; // no shifting
}

void tA0_B1(int* mat1, int* mat2, int* mat3, int r, int c){
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[i*r*2 + j+c] += mat1[i*r*2 + k] * mat2[(j+r)*c*2 + k]; // keep A, row B
}

void tA1_B2(int* mat1, int* mat2, int* mat3, int r, int c){
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[i*r*2 + j] += mat1[i*r*2 + k+c] * mat2[j*c*2 + k+c]; // col A, col B
}

void tA1_B3(int* mat1, int* mat2, int* mat3, int r, int c){
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[i*r*2 + j+c] += mat1[i*r*2 + k+c] * mat2[(j+r)*c*2 + k+c]; // col A, rowB col B
}

void tA2_B0(int* mat1, int* mat2, int* mat3, int r, int c){
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[(i+r)*r*2 + j] += mat1[(i+r)*r*2 + k] * mat2[j*c*2 + k]; // row A
}

void tA2_B1(int* mat1, int* mat2, int* mat3, int r, int c){
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[(i+r)*r*2 + j+c] += mat1[(i+r)*r*2 + k] * mat2[(j+r)*c*2 + k]; // row A, row B
}

void tA3_B2(int* mat1, int* mat2, int* mat3, int r, int c){
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[(i+r)*r*2 + j] += mat1[(i+r)*r*2 + k+c] * mat2[j*c*2 + k+c]; // col row A, col B
}

void tA3_B3(int* mat1, int* mat2, int* mat3, int r, int c){
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[(i+r)*r*2 + j+c] += mat1[(i+r)*r*2 + k+c] * mat2[(j+r)*c*2 + k+c]; // col row A, col row B
}

void blocked_transpose(int* mat1, int* mat2, int* mat3, int r, int c){
  std::chrono::duration<double> diff;
  auto start_time = std::chrono::steady_clock::now();
  int r2 = r/2;
  int c2 = c/2;
  int* transposed = generate_0(r,c);
  transpose(mat2, transposed,r,c);


//  A0_B0(mat1, transposed, mat3, r2,c2);
//  A1_B2(mat1, transposed, mat3, r2,c2);
//  A0_B1(mat1, transposed, mat3, r2,c2);
//  A1_B3(mat1, transposed, mat3, r2,c2);
//  A2_B0(mat1, transposed, mat3, r2,c2);
//  A3_B2(mat1, transposed, mat3, r2,c2);
//  A2_B1(mat1, transposed, mat3, r2,c2);
//  A3_B3(mat1, transposed, mat3, r2,c2);
  
  std::thread t0(tA0_B0, mat1, transposed, mat3, r2,c2);
  std::thread t2(tA0_B1, mat1, transposed, mat3, r2,c2);
  std::thread t4(tA2_B0, mat1, transposed, mat3, r2,c2);
  std::thread t6(tA2_B1, mat1, transposed, mat3, r2,c2);
  t0.join();t2.join();t4.join();t6.join();
  
  
  
  std::thread t1(tA1_B2, mat1, transposed, mat3, r2,c2);
  std::thread t3(tA1_B3, mat1, transposed, mat3, r2,c2);
  std::thread t5(tA3_B2, mat1, transposed, mat3, r2,c2);
  std::thread t7(tA3_B3, mat1, transposed, mat3, r2,c2);
  
  
  t1.join();
  t3.join();
  t5.join();
  t7.join();
  
  auto end_time = std::chrono::steady_clock::now();
  diff = end_time - start_time;
  std::cout << "B Matrix took: " << diff.count() << " sec" << std::endl;
}
