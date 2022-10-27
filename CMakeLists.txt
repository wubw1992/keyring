project(keyring)
cmake_minimum_required(VERSION 3.13)
find_package(PkgConfig REQUIRED)

pkg_check_modules(secret REQUIRED IMPORTED_TARGET libsecret-1)
add_compile_definitions(SECRET_API_SUBJECT_TO_CHANGE)
add_executable(keyring keyring.c)
target_link_libraries(keyring PRIVATE PkgConfig::secret)

