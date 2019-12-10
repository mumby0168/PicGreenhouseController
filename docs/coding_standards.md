# Coding Standards
## Standard Data Type Prefixies
```c
int iVariableName;
short sVariableName;
long lVariableName;
long long llVariableName;
float fVariableName;
double dVariableName;
long double ldVariableName;
unsigned x uxVariableName;
x* pVariableName;
x axVariableName[];
const x = xVariableName;
```
Where x is any of the other types listed.
### Examples

# Global Variable
```c
struct DataType g_variableName;
int g_iVariableName;
```

# Static Variables
```c
static int s_iVariableName;
```

# Static Functions
```c
static TYPE file_name_function_name(/*...*/);
```

# Functions
```c
TYPE File_Name_FunctionName(/*...*/);
```

# Struct Declarations and Definitions
```c
typedef struct _struct_name StructName;
typedef struct _struct_name
{
    /*...*/
} StructName;
struct _struct_name
{
    /*...*/
};
```

#  Declarations and Definitions
```c
typedef enum _enum_name EnumName;
typedef enum _enum_name
{
    ENUM_NAME_ITEM_ONE = 1,
    /*...*/
} EnumName;
enum _enum_name
{
    ENUM_NAME_ITEM_ONE = 1,
    /*...*/
};
```