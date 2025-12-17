unsigned __int64 __fastcall sub_140978520(__int64 a1)
{
  unsigned __int64 v1; // rdx
  __int64 v3; // r8
  unsigned __int64 v4; // rax
  int v5; // r9d
  __int64 v6; // rdx
  __int16 v7; // [rsp+30h] [rbp+8h] BYREF
  char v8; // [rsp+32h] [rbp+Ah]
  char v9; // [rsp+33h] [rbp+Bh]
  __int64 v10; // [rsp+38h] [rbp+10h] BYREF

  v1 = *(_QWORD *)(a1 + 400);
  if ( !v1 )
    return 0LL;
  v3 = HIWORD(v1) & 0x7FFF;
  v4 = v1 >> 63;
  v5 = dword_154DEA6C0 & (1 << SBYTE6(v1));
  v6 = v1 & 0xFFFFFFFFFFFFLL;
  v10 = v6;
  if ( (_BYTE)v4 && v5 )
  {
    if ( dword_1542F5410 == 1 )
    {
      sub_14094D600(qword_154DEA730, &v10, 4LL, (unsigned int)v3);
    }
    else
    {
      v9 &= 0xFCu;
      v7 = v3;
      v8 = 0;
      off_154DBF328(&v10, 4LL, &v7);
      if ( (v9 & 1) != 0 )
        off_154DBF338(&v7);
    }
    v6 = v10 & 0xFFFFFFFFFFFFLL;
    if ( (v10 & 0xFFFFFFFFFFFFuLL) >= 0x800000000000LL )
      return v10 & 0xFFFFFFFFFFFFLL | 0xFFFF000000000000uLL;
  }
  return v6;
}