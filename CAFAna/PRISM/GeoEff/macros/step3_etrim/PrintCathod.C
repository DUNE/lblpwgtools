void PrintCathod(){

for (int i = -3; i <= 3; ++i) {
  // 0.5cm cathode in the middle of each module, plus 0.5cm buffer
  double cathode_center = i * 102.1;
  double module_boundary = i * 102.1 + 51.05;
  cout<<" cathode center: "<<cathode_center <<" cc -0.75 "<<cathode_center - 0.75<<" cc+0.75 "<<cathode_center + 0.75<<" module boundary: "<< module_boundary<<endl;

  }
}
