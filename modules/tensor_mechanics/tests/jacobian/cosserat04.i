[Mesh]
  type = GeneratedMesh
  dim = 3
[]

[GlobalParams]
  disp_z = disp_z
  disp_y = disp_y
  disp_x = disp_x
  wc_z = wc_z
  wc_y = wc_y
  wc_x = wc_x
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]
  [./wc_x]
  [../]
  [./wc_y]
  [../]
  [./wc_z]
  [../]
[]

[Kernels]
  active = 'cx_elastic cy_elastic cz_elastic x_couple y_couple z_couple x_moment y_moment z_moment'
  [./cx_elastic]
    type = CosseratStressDivergenceTensors
    variable = disp_x
    component = 0
  [../]
  [./cy_elastic]
    type = CosseratStressDivergenceTensors
    variable = disp_y
    component = 1
  [../]
  [./cz_elastic]
    type = CosseratStressDivergenceTensors
    variable = disp_z
    component = 2
  [../]
  [./x_couple]
    type = StressDivergenceTensors
    variable = wc_x
    disp_z = wc_z
    disp_y = wc_y
    disp_x = wc_x
    component = 0
    appended_property_name = _couple
  [../]
  [./y_couple]
    type = StressDivergenceTensors
    variable = wc_y
    disp_z = wc_z
    disp_y = wc_y
    disp_x = wc_x
    component = 1
    appended_property_name = _couple
  [../]
  [./z_couple]
    type = StressDivergenceTensors
    variable = wc_z
    disp_z = wc_z
    disp_y = wc_y
    disp_x = wc_x
    component = 2
    appended_property_name = _couple
  [../]
  [./x_moment]
    type = MomentBalancing
    variable = wc_x
    component = 0
  [../]
  [./y_moment]
    type = MomentBalancing
    variable = wc_y
    component = 1
  [../]
  [./z_moment]
    type = MomentBalancing
    variable = wc_z
    component = 2
  [../]
[]


[Materials]
  [./cosserat]
    type = CosseratLinearElasticMaterial
    block = 0
    B_ijkl = '1 2.2 2.333 1.988 1 2.1 2.2 2.3 2.4 1 2.2 2.333 1.988 1 2.1 2.2 2.3 2.4 2.2 2 1.6'
    fill_method_bending = 'symmetric21'
    C_ijkl = '1.07 1.2 1.333 0.988 1.123 1.1 1.25 1.3 1.4 1 1.2 1.333 0.9 1.11 1.16 1.28 1.35 1.45 1.03 1 0.6'
    fill_method = 'symmetric21'
  [../]
[]

[Preconditioning]
  [./andy]
    type = SMP
    full = true
    #petsc_options = '-snes_test_display'
    petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -snes_type'
    petsc_options_value = 'bcgs bjacobi 1E-15 1E-10 10000 test'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = Newton
[]

[Outputs]
  console = true
[]

