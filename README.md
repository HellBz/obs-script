# obs-script

A plugin for OBS Studio that allows for Lua to control elements of the application. Ideally one would be able to create and manage scenes based on arbitrary criteria, as well as manage the sources within those scenes based on whatever conditions they deem fit.

### General Rules
* Almost Always Auto
    * prefer auto on left hand types
* Prefer Const
    * a good rule of thumb
* Prefer using over typedef
* Name template with a T
    * ```template <typename TArg>```
* CamelCase for classes and structs
* Match namespaces to folder structure
    * this allows for simpler naming
    * also tends to remove things like underscores from names
* m_ for members and then camelCase
* camelCase for parameters and locals
* Any non-default constructor should be explicit
* use override
* Prefer to use constexpr and typed versions of type_traits
    * ```std::is_same_v<T>``` over ```std::is_same<T>::value```
    * ```std::remove_const_t<T>``` over ```typename std::remove_const<T>::type```
    * etc
* Use clang-format
    * I like to have consistent code, try not to deviate from the format