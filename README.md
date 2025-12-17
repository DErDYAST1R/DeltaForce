# DeltaForce

Delta Force Usermode \& SDK With Decryption as of 12.17.2025



Decrypt function as of 12.17.2025 from the main binary file.



```cpp
unsigned \_\_int64 \_\_fastcall sub\_140978520(\_\_int64 a1)

{

&nbsp; unsigned \_\_int64 v1; // rdx

&nbsp; \_\_int64 v3; // r8

&nbsp; unsigned \_\_int64 v4; // rax

&nbsp; int v5; // r9d

&nbsp; \_\_int64 v6; // rdx

&nbsp; \_\_int16 v7; // \[rsp+30h] \[rbp+8h] BYREF

&nbsp; char v8; // \[rsp+32h] \[rbp+Ah]

&nbsp; char v9; // \[rsp+33h] \[rbp+Bh]

&nbsp; \_\_int64 v10; // \[rsp+38h] \[rbp+10h] BYREF



&nbsp; v1 = \*(\_QWORD \*)(a1 + 400);

&nbsp; if ( !v1 )

&nbsp;   return 0LL;

&nbsp; v3 = HIWORD(v1) \& 0x7FFF;

&nbsp; v4 = v1 >> 63;

&nbsp; v5 = dword\_154DEA6C0 \& (1 << SBYTE6(v1));

&nbsp; v6 = v1 \& 0xFFFFFFFFFFFFLL;

&nbsp; v10 = v6;

&nbsp; if ( (\_BYTE)v4 \&\& v5 )

&nbsp; {

&nbsp;   if ( dword\_1542F5410 == 1 )

&nbsp;   {

&nbsp;     sub\_14094D600(qword\_154DEA730, \&v10, 4LL, (unsigned int)v3);

&nbsp;   }

&nbsp;   else

&nbsp;   {

&nbsp;     v9 \&= 0xFCu;

&nbsp;     v7 = v3;

&nbsp;     v8 = 0;

&nbsp;     off\_154DBF328(\&v10, 4LL, \&v7);

&nbsp;     if ( (v9 \& 1) != 0 )

&nbsp;       off\_154DBF338(\&v7);

&nbsp;   }

&nbsp;   v6 = v10 \& 0xFFFFFFFFFFFFLL;

&nbsp;   if ( (v10 \& 0xFFFFFFFFFFFFuLL) >= 0x800000000000LL )

&nbsp;     return v10 \& 0xFFFFFFFFFFFFLL | 0xFFFF000000000000uLL;

&nbsp; }

&nbsp; return v6;

}
```

