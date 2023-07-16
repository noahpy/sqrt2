
#include "../big_num/big_num.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_string(char *string, size_t len) {
  for (size_t i = 0; i < len / 2; i++) {
    char temp = string[i];
    string[i] = string[len - i - 1];
    string[len - i - 1] = temp;
  }
}

char get_hex_value(char hex) {
  if (hex >= '0' && hex <= '9') {
    return hex - '0';
  }
  if (hex >= 'A' && hex <= 'F') {
    return hex - 'A' + 10;
  }
  if (hex >= 'a' && hex <= 'f') {
    return hex - 'a' + 10;
  }
  fprintf(stderr, "Error: Invalid hex character\n");
  exit(EXIT_FAILURE);
}


char *hex_to_dec(char *hex, size_t len, size_t fraction_len, bool is_sqrt2) {
  // determine the length of the decimal string with magic number 1.204119983
  // (ln(16) / ln(10))
  size_t string_len;
  size_t fraction = (size_t)ceil(((double)len) * 0.204119983);
  if (__builtin_add_overflow(len, fraction, &string_len)) {
    fprintf(stderr,
            "Error: Could not respresent the length of the decimal string\n");
    exit(EXIT_FAILURE);
  }
  // allocate memory for the decimal string (with extra space for dot, zero and
  // terminating null byte)
  char *result = malloc(string_len + 3);
  if (result == NULL) {
    fprintf(stderr,
            "Error: Could not allocate memory for the decimal string\n");
    exit(EXIT_FAILURE);
  }
  // convert the hex string to decimal
  size_t result_len = 0;
  for (size_t i = 0; i < len; i++) {
    int carry = get_hex_value(hex[i]);
    for (size_t j = 0; j < result_len; j++) {
      int digit = result[j] * 16 + carry;
      result[j] = (char)(digit % 10);
      carry = digit / 10;
    }
    while (carry) {
      result[result_len++] = (char)(carry % 10);
      carry /= 10;
    }
  }
  // format the result string
  reverse_string(result, result_len);
  for (size_t i = 0; i < result_len; i++) {
    result[i] += '0';
  }

  if (is_sqrt2)
    return result;

  if (result_len < fraction_len) {
    fprintf(stderr,
            "Error: The fraction length is greater than the decimal length\n");
    fprintf(stderr,
            "Fraction length was %zu, but only %zu digits were computed\n",
            fraction_len, result_len);
    fprintf(stderr, "Here is still what was computed: \n");
    fraction_len = result_len;
    /* exit(EXIT_FAILURE); */
  }
  size_t offset = (result_len - fraction_len) ? 0 : 1;

  // add dot, zero and remove trailing zeros
  if (fraction_len) {
    bool trailing = true;
    size_t index = result_len - 1;
    while (index >= result_len - fraction_len && index < result_len) {
      if (result[index] != '0' || !trailing) {
        if (trailing)
          result[index + 2 + offset] = '\0';
        result[index + 1 + offset] = result[index];
        trailing = false;
      }
      index--;
    }
    result[index + 1 + offset] = '.';
    if (offset) {
      result[0] = '0';
    }
  }

  // add terminating null byte
  if (!fraction_len)
    result[result_len] = '\0';
  return result;
}

char *bin_to_hex(struct bignum num) {
  size_t string_len;
  if (__builtin_umull_overflow(num.size, 8, &string_len)) {
    fprintf(stderr,
            "Error: Could not represent the length of the hex string\n");
    exit(EXIT_FAILURE);
  }
  char *result = malloc(string_len + 1);
  if (result == NULL) {
    fprintf(stderr, "Error: Could not allocate memory for the hex string\n");
    exit(EXIT_FAILURE);
  }
  char *current = result;
  for (int i = num.size - 1; i > -1; i--) {
    sprintf(current, "%08x", num.digits[i]);
    current += 8;
  }
  // add terminating null byte
  result[string_len] = '\0';
  return result;
}

void print_bignum_dec(struct bignum *num,
                      struct bignum multiply(struct bignum, struct bignum),
                      long isSqrt2) {
  struct bignum result = *num;
  if (num->fracSize) {
    size_t frac_hex = num->fracSize / 4;
    struct bignum multiplier = bignumOfInt(10000);
    struct bignum double_multiplier = {0, 0, 0};
    struct bignum new_result;

    while (frac_hex > 1024) {
      if (!double_multiplier.digits) {
        // 10^512 =
        // 0x1c__633415d4_c1d238d9_8cab8a97_8a0b1f13_8cb07303_a2699748_45a71d46_b099bc81_7343afac_
        // 69be5b0e_9449775c_1366732a_93abade4_b2908ee0_f95f635e_85a91924_c3fc0695_e7fc7153_
        // 329c57ae_bfa3edac_96e14f5d_bc51fb2e_b21a2f22_1e25cfea_703ed321_aa1da1bf_28f8733b_
        // 4475b579_c88976c1_94e65747_46c40513_c31e1ad9_b83a8a97_5d96976f_8f9546dc_77f27267_
        // fc6cf801_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_
        // 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000
        uint32_t *digits = allocateDigits(426);
        digits[0] = 0x00000000;
        digits[1] = 0x00000000;
        digits[2] = 0x00000000;
        digits[3] = 0x00000000;
        digits[4] = 0x00000000;
        digits[5] = 0x00000000;
        digits[6] = 0x00000000;
        digits[7] = 0x00000000;
        digits[8] = 0x00000000;
        digits[9] = 0x00000000;
        digits[10] = 0x00000000;
        digits[11] = 0x00000000;
        digits[12] = 0x00000000;
        digits[13] = 0x00000000;
        digits[14] = 0x00000000;
        digits[15] = 0x00000000;
        digits[16] = 0x00000000;
        digits[17] = 0x00000000;
        digits[18] = 0x00000000;
        digits[19] = 0x00000000;
        digits[20] = 0x00000000;
        digits[21] = 0x00000000;
        digits[22] = 0x00000000;
        digits[23] = 0x00000000;
        digits[24] = 0x00000000;
        digits[25] = 0x00000000;
        digits[26] = 0x00000000;
        digits[27] = 0x00000000;
        digits[28] = 0x00000000;
        digits[29] = 0x00000000;
        digits[30] = 0x00000000;
        digits[31] = 0x00000000;
        digits[32] = 0x00000000;
        digits[33] = 0x00000000;
        digits[34] = 0x00000000;
        digits[35] = 0x00000000;
        digits[36] = 0x00000000;
        digits[37] = 0x00000000;
        digits[38] = 0x00000000;
        digits[39] = 0x00000000;
        digits[40] = 0x00000000;
        digits[41] = 0x00000000;
        digits[42] = 0x00000000;
        digits[43] = 0x00000000;
        digits[44] = 0x00000000;
        digits[45] = 0x00000000;
        digits[46] = 0x00000000;
        digits[47] = 0x00000000;
        digits[48] = 0x00000000;
        digits[49] = 0x00000000;
        digits[50] = 0x00000000;
        digits[51] = 0x00000000;
        digits[52] = 0x00000000;
        digits[53] = 0x00000000;
        digits[54] = 0x00000000;
        digits[55] = 0x00000000;
        digits[56] = 0x00000000;
        digits[57] = 0x00000000;
        digits[58] = 0x00000000;
        digits[59] = 0x00000000;
        digits[60] = 0x00000000;
        digits[61] = 0x00000000;
        digits[62] = 0x00000000;
        digits[63] = 0x00000000;
        digits[64] = 0x00000000;
        digits[65] = 0x00000000;
        digits[66] = 0x00000000;
        digits[67] = 0x00000000;
        digits[68] = 0x00000000;
        digits[69] = 0x00000000;
        digits[70] = 0x00000000;
        digits[71] = 0x00000000;
        digits[72] = 0x00000000;
        digits[73] = 0x00000000;
        digits[74] = 0x00000000;
        digits[75] = 0x00000000;
        digits[76] = 0x00000000;
        digits[77] = 0x00000000;
        digits[78] = 0x00000000;
        digits[79] = 0x00000000;
        digits[80] = 0x00000000;
        digits[81] = 0x00000000;
        digits[82] = 0x00000000;
        digits[83] = 0x00000000;
        digits[84] = 0x00000000;
        digits[85] = 0x00000000;
        digits[86] = 0x00000000;
        digits[87] = 0x00000000;
        digits[88] = 0x00000000;
        digits[89] = 0x00000000;
        digits[90] = 0x00000000;
        digits[91] = 0x00000000;
        digits[92] = 0x00000000;
        digits[93] = 0x00000000;
        digits[94] = 0x00000000;
        digits[95] = 0x00000000;
        digits[96] = 0x00000000;
        digits[97] = 0x00000000;
        digits[98] = 0x00000000;
        digits[99] = 0x00000000;
        digits[100] = 0x00000000;
        digits[101] = 0x00000000;
        digits[102] = 0x00000000;
        digits[103] = 0x00000000;
        digits[104] = 0x00000000;
        digits[105] = 0x00000000;
        digits[106] = 0x00000000;
        digits[107] = 0x00000000;
        digits[108] = 0x00000000;
        digits[109] = 0x00000000;
        digits[110] = 0x00000000;
        digits[111] = 0x00000000;
        digits[112] = 0x00000000;
        digits[113] = 0x00000000;
        digits[114] = 0x00000000;
        digits[115] = 0x00000000;
        digits[116] = 0x00000000;
        digits[117] = 0x00000000;
        digits[118] = 0x00000000;
        digits[119] = 0x00000000;
        digits[120] = 0x00000000;
        digits[121] = 0x00000000;
        digits[122] = 0x00000000;
        digits[123] = 0x00000000;
        digits[124] = 0x00000000;
        digits[125] = 0x00000000;
        digits[126] = 0x00000000;
        digits[127] = 0x00000000;
        digits[128] = 0x2a67c001;
        digits[129] = 0xd4724e8d;
        digits[130] = 0x8efe7ae7;
        digits[131] = 0xf89a1e90;
        digits[132] = 0xef084117;
        digits[133] = 0x54e05154;
        digits[134] = 0x13b1bb51;
        digits[135] = 0x506be829;
        digits[136] = 0xfb29b172;
        digits[137] = 0xe599574e;
        digits[138] = 0xf0da6146;
        digits[139] = 0x806c0ed3;
        digits[140] = 0xb86ae5be;
        digits[141] = 0x45155e93;
        digits[142] = 0xc0591cc2;
        digits[143] = 0x7e1e7c34;
        digits[144] = 0x7c4823da;
        digits[145] = 0x1d1f4cce;
        digits[146] = 0x9b8ba1e8;
        digits[147] = 0xd6bfdf75;
        digits[148] = 0xe341be10;
        digits[149] = 0xc2dfae78;
        digits[150] = 0x016b67b2;
        digits[151] = 0x0f237f1a;
        digits[152] = 0x3dbeabcd;
        digits[153] = 0xaf6a2574;
        digits[154] = 0xcab3e6d7;
        digits[155] = 0x142e0e80;
        digits[156] = 0x61959127;
        digits[157] = 0x2c234811;
        digits[158] = 0x87009701;
        digits[159] = 0xcb4bf982;
        digits[160] = 0xf8169c84;
        digits[161] = 0x88052f8c;
        digits[162] = 0x68dde6d4;
        digits[163] = 0xbc131761;
        digits[164] = 0xff0b0905;
        digits[165] = 0x54ab9c41;
        digits[166] = 0x7613b224;
        digits[167] = 0x1a1c304e;
        digits[168] = 0x3bfe167b;
        digits[169] = 0x441c2d47;
        digits[170] = 0x4f6cea9c;
        digits[171] = 0x78f06181;
        digits[172] = 0xeb659fb8;
        digits[173] = 0x30c7ae41;
        digits[174] = 0x947e0d0e;
        digits[175] = 0xa1ebcad7;
        digits[176] = 0xd97d9556;
        digits[177] = 0x2130504d;
        digits[178] = 0x1a8309cb;
        digits[179] = 0xf2acd507;
        digits[180] = 0x3f8ec72a;
        digits[181] = 0xfd82373a;
        digits[182] = 0x95a842bc;
        digits[183] = 0x280f4d32;
        digits[184] = 0xf3618ac0;
        digits[185] = 0x811a4f04;
        digits[186] = 0x6dc3a5b4;
        digits[187] = 0xd3967a1b;
        digits[188] = 0x15b8c898;
        digits[189] = 0xdcfe388f;
        digits[190] = 0x454eb2a0;
        digits[191] = 0x8738b909;
        digits[192] = 0x10c4e996;
        digits[193] = 0x2bd9cc11;
        digits[194] = 0x3297cd0c;
        digits[195] = 0x655fec30;
        digits[196] = 0xae0725b1;
        digits[197] = 0xf4090ee8;
        digits[198] = 0x037d19ee;
        digits[199] = 0x398c6fed;
        digits[200] = 0x3b9af26b;
        digits[201] = 0xc994a450;
        digits[202] = 0xb5341743;
        digits[203] = 0x75a697b2;
        digits[204] = 0xac50b9c1;
        digits[205] = 0x3ccb5b92;
        digits[206] = 0xffe06205;
        digits[207] = 0xa8329761;
        digits[208] = 0xdfea5242;
        digits[209] = 0xeb83cadb;
        digits[210] = 0xe79dadf7;
        digits[211] = 0x3c20ee69;
        digits[212] = 0x1e0a6817;
        digits[213] = 0x7021b97a;
        digits[214] = 0x743074fa;
        digits[215] = 0x176ca776;
        digits[216] = 0x77fb8af6;
        digits[217] = 0xeca19beb;
        digits[218] = 0x92baf1de;
        digits[219] = 0xaf63b712;
        digits[220] = 0xde35c88b;
        digits[221] = 0xa4eb8f8c;
        digits[222] = 0xe137d5e9;
        digits[223] = 0x40b464a0;
        digits[224] = 0x87d1cde8;
        digits[225] = 0x42923bbd;
        digits[226] = 0xcd8f62ff;
        digits[227] = 0x2e2690f3;
        digits[228] = 0x095edc16;
        digits[229] = 0x59c89f1b;
        digits[230] = 0x1fa8fd5d;
        digits[231] = 0x5138753d;
        digits[232] = 0x390a2b29;
        digits[233] = 0x80152f18;
        digits[234] = 0x2dd8d925;
        digits[235] = 0xf984d83e;
        digits[236] = 0x7a872e74;
        digits[237] = 0xc19e1faf;
        digits[238] = 0xed4d542d;
        digits[239] = 0xecf9b5d0;
        digits[240] = 0x9462ea75;
        digits[241] = 0xc53c0adf;
        digits[242] = 0x0caea134;
        digits[243] = 0x37a2d439;
        digits[244] = 0xc8fa2e8a;
        digits[245] = 0x2181327e;
        digits[246] = 0x6e7bb827;
        digits[247] = 0x2d240820;
        digits[248] = 0x50be10e0;
        digits[249] = 0x5893d4b8;
        digits[250] = 0xab312bb9;
        digits[251] = 0x1f2b2322;
        digits[252] = 0x440b3f25;
        digits[253] = 0xbf627ede;
        digits[254] = 0x72dac789;
        digits[255] = 0xb608b895;
        digits[256] = 0x78787e2a;
        digits[257] = 0x86deb3f0;
        digits[258] = 0x6fee7aab;
        digits[259] = 0xbb9373f4;
        digits[260] = 0x27ecf57b;
        digits[261] = 0xf7d8b57e;
        digits[262] = 0xfca26a9f;
        digits[263] = 0x3d04e8d2;
        digits[264] = 0xc9df13cb;
        digits[265] = 0x3172826a;
        digits[266] = 0xcd9e8d7c;
        digits[267] = 0xa8fcd8e0;
        digits[268] = 0xb2c39497;
        digits[269] = 0x307641d9;
        digits[270] = 0x1cc939c1;
        digits[271] = 0x2608c4cf;
        digits[272] = 0xb6d1c7bf;
        digits[273] = 0x3d326a7e;
        digits[274] = 0xeeaf19e6;
        digits[275] = 0x8e13e25f;
        digits[276] = 0xee63302b;
        digits[277] = 0x2dfe6d97;
        digits[278] = 0x25971d58;
        digits[279] = 0xe41d3cc4;
        digits[280] = 0x0a80627c;
        digits[281] = 0xab8db59a;
        digits[282] = 0x9eea37c8;
        digits[283] = 0xe90afb77;
        digits[284] = 0x90ca19cf;
        digits[285] = 0x9ee3352c;
        digits[286] = 0x3613c850;
        digits[287] = 0xfe78d682;
        digits[288] = 0x788f6e50;
        digits[289] = 0x5b060904;
        digits[290] = 0xb71bd1a4;
        digits[291] = 0x3fecb534;
        digits[292] = 0xb32c450c;
        digits[293] = 0x20c33857;
        digits[294] = 0xa6e9cfda;
        digits[295] = 0x0239f4ce;
        digits[296] = 0x48497187;
        digits[297] = 0xa19adb95;
        digits[298] = 0xb492ed8a;
        digits[299] = 0x95aca6a8;
        digits[300] = 0x4dcd6cd9;
        digits[301] = 0xcf1b2350;
        digits[302] = 0xfbe8b12a;
        digits[303] = 0x1a67778c;
        digits[304] = 0x38eb3acc;
        digits[305] = 0xc32da383;
        digits[306] = 0xfb126ab1;
        digits[307] = 0xa03f40a8;
        digits[308] = 0xed5bf546;
        digits[309] = 0xe9ce4724;
        digits[310] = 0x4c4a74fd;
        digits[311] = 0x73a130d8;
        digits[312] = 0xd9960e2d;
        digits[313] = 0xa2ebd6c1;
        digits[314] = 0x94ab6feb;
        digits[315] = 0x6f233b7c;
        digits[316] = 0x49126080;
        digits[317] = 0x8e7b9a73;
        digits[318] = 0x4b8c9091;
        digits[319] = 0xd298f999;
        digits[320] = 0x35e836b5;
        digits[321] = 0xa96ddeff;
        digits[322] = 0x96119b31;
        digits[323] = 0x6b0dd9bc;
        digits[324] = 0xc6cc3f8d;
        digits[325] = 0x282566fb;
        digits[326] = 0x72b882e7;
        digits[327] = 0xd6769f3b;
        digits[328] = 0xa674343d;
        digits[329] = 0x00fc509b;
        digits[330] = 0xdcbf7789;
        digits[331] = 0xd6266a3f;
        digits[332] = 0xae9641fd;
        digits[333] = 0x4e89541b;
        digits[334] = 0x11953407;
        digits[335] = 0x53400d03;
        digits[336] = 0x8e0dd75a;
        digits[337] = 0xe5b53345;
        digits[338] = 0x108f19ad;
        digits[339] = 0x108b89bc;
        digits[340] = 0x41a4c954;
        digits[341] = 0xe03b2b63;
        digits[342] = 0x437b3d7f;
        digits[343] = 0x97aced8e;
        digits[344] = 0xcbd66670;
        digits[345] = 0x2c5508c2;
        digits[346] = 0x650ebc69;
        digits[347] = 0x5c4f2ef0;
        digits[348] = 0x904ff6bf;
        digits[349] = 0x9985a2df;
        digits[350] = 0x9faddd9e;
        digits[351] = 0x5ed8d239;
        digits[352] = 0x25585832;
        digits[353] = 0xe3e51cb9;
        digits[354] = 0x0ff4f1d4;
        digits[355] = 0x56c02d9a;
        digits[356] = 0x8c4ef804;
        digits[357] = 0xc1a08a13;
        digits[358] = 0x13fd01c8;
        digits[359] = 0xe6d27671;
        digits[360] = 0xa7c234f4;
        digits[361] = 0x9d0176cc;
        digits[362] = 0xd0d73df2;
        digits[363] = 0x4d8bfa89;
        digits[364] = 0x544f10cd;
        digits[365] = 0x2b17e0b2;
        digits[366] = 0xb70a5c7d;
        digits[367] = 0xfd86fe49;
        digits[368] = 0xdf373f41;
        digits[369] = 0x214495bb;
        digits[370] = 0x84e857fd;
        digits[371] = 0x00d313d5;
        digits[372] = 0x0496fcbe;
        digits[373] = 0xa4ba4744;
        digits[374] = 0xe8cac982;
        digits[375] = 0xaec29e6e;
        digits[376] = 0x87ec7038;
        digits[377] = 0x7000a519;
        digits[378] = 0xaeee333b;
        digits[379] = 0xff66e42c;
        digits[380] = 0x8afd6b25;
        digits[381] = 0x03b4f63b;
        digits[382] = 0xbd7991dc;
        digits[383] = 0x5ab8d9c7;
        digits[384] = 0x2ed4684e;
        digits[385] = 0x48741a6c;
        digits[386] = 0xaf06940d;
        digits[387] = 0x2fdc6349;
        digits[388] = 0xb03d7ecd;
        digits[389] = 0xe974996f;
        digits[390] = 0xac7867f9;
        digits[391] = 0x52ec8721;
        digits[392] = 0xbcdd9d4a;
        digits[393] = 0x8edd2d00;
        digits[394] = 0x3557de06;
        digits[395] = 0x41c759f8;
        digits[396] = 0x3956d4b9;
        digits[397] = 0xa75409f2;
        digits[398] = 0x123cd8a1;
        digits[399] = 0xb6100fab;
        digits[400] = 0x3e7b21e2;
        digits[401] = 0x2e8d623b;
        digits[402] = 0x92959da2;
        digits[403] = 0xbca35f77;
        digits[404] = 0x200c03a5;
        digits[405] = 0x35fcb457;
        digits[406] = 0x1bb6c6e4;
        digits[407] = 0xf74eb928;
        digits[408] = 0x3d5d0b54;
        digits[409] = 0x87cc1d21;
        digits[410] = 0x4964046f;
        digits[411] = 0x18ae4240;
        digits[412] = 0xd868b275;
        digits[413] = 0x8bd2b496;
        digits[414] = 0x1c5563f4;
        digits[415] = 0xc234d8f5;
        digits[416] = 0xf868e970;
        digits[417] = 0xf9151fff;
        digits[418] = 0xae7be4a2;
        digits[419] = 0x271133ee;
        digits[420] = 0xbb0fd922;
        digits[421] = 0x25254932;
        digits[422] = 0xa60a9fc0;
        digits[423] = 0x104bcd64;
        digits[424] = 0x30290145;
        digits[425] = 0x62;
        double_multiplier.digits = digits;
        double_multiplier.size = 426;
        double_multiplier.fracSize = 0;
      }
      new_result = multiply(result, double_multiplier);
      frac_hex -= 1024;
      if (result.digits != num->digits)
        free(result.digits);
      result = new_result;
    }
    free(double_multiplier.digits);
    double_multiplier.digits = NULL;

    while (frac_hex > 128) {
      if (!double_multiplier.digits) {
        // 10^512 =
        // 0x1c__633415d4_c1d238d9_8cab8a97_8a0b1f13_8cb07303_a2699748_45a71d46_b099bc81_7343afac_
        // 69be5b0e_9449775c_1366732a_93abade4_b2908ee0_f95f635e_85a91924_c3fc0695_e7fc7153_
        // 329c57ae_bfa3edac_96e14f5d_bc51fb2e_b21a2f22_1e25cfea_703ed321_aa1da1bf_28f8733b_
        // 4475b579_c88976c1_94e65747_46c40513_c31e1ad9_b83a8a97_5d96976f_8f9546dc_77f27267_
        // fc6cf801_00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000_
        // 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000
        uint32_t *digits = allocateDigits(54);
        digits[0] = 0x00000000;
        digits[1] = 0x00000000;
        digits[2] = 0x00000000;
        digits[3] = 0x00000000;
        digits[4] = 0x00000000;
        digits[5] = 0x00000000;
        digits[6] = 0x00000000;
        digits[7] = 0x00000000;
        digits[8] = 0x00000000;
        digits[9] = 0x00000000;
        digits[10] = 0x00000000;
        digits[11] = 0x00000000;
        digits[12] = 0x00000000;
        digits[13] = 0x00000000;
        digits[14] = 0x00000000;
        digits[15] = 0x00000000;
        digits[16] = 0xfc6cf801;
        digits[17] = 0x77f27267;
        digits[18] = 0x8f9546dc;
        digits[19] = 0x5d96976f;
        digits[20] = 0xb83a8a97;
        digits[21] = 0xc31e1ad9;
        digits[22] = 0x46c40513;
        digits[23] = 0x94e65747;
        digits[24] = 0xc88976c1;
        digits[25] = 0x4475b579;
        digits[26] = 0x28f8733b;
        digits[27] = 0xaa1da1bf;
        digits[28] = 0x703ed321;
        digits[29] = 0x1e25cfea;
        digits[30] = 0xb21a2f22;
        digits[31] = 0xbc51fb2e;
        digits[32] = 0x96e14f5d;
        digits[33] = 0xbfa3edac;
        digits[34] = 0x329c57ae;
        digits[35] = 0xe7fc7153;
        digits[36] = 0xc3fc0695;
        digits[37] = 0x85a91924;
        digits[38] = 0xf95f635e;
        digits[39] = 0xb2908ee0;
        digits[40] = 0x93abade4;
        digits[41] = 0x1366732a;
        digits[42] = 0x9449775c;
        digits[43] = 0x69be5b0e;
        digits[44] = 0x7343afac;
        digits[45] = 0xb099bc81;
        digits[46] = 0x45a71d46;
        digits[47] = 0xa2699748;
        digits[48] = 0x8cb07303;
        digits[49] = 0x8a0b1f13;
        digits[50] = 0x8cab8a97;
        digits[51] = 0xc1d238d9;
        digits[52] = 0x633415d4;
        digits[53] = 0x1c;
        double_multiplier.digits = digits;
        double_multiplier.size = 54;
        double_multiplier.fracSize = 0;
      }
      new_result = multiply(result, double_multiplier);
      frac_hex -= 128;
      if (result.digits != num->digits)
        free(result.digits);
      result = new_result;
    }
    free(double_multiplier.digits);
    double_multiplier.digits = NULL;

    while (frac_hex > 16) {
      if (!double_multiplier.digits) {
        // 10^64 =
        // 0x184F03_E93FF9F4_DAA797ED_6E38ED64_BF6A1F01_00000000_00000000
        uint32_t *digits = allocateDigits(7);
        digits[0] = 0;
        digits[1] = 0;
        digits[2] = 0xBF6A1F01;
        digits[3] = 0x6E38ED64;
        digits[4] = 0xDAA797ED;
        digits[5] = 0xE93FF9F4;
        digits[6] = 0x184F03;
        double_multiplier.digits = digits;
        double_multiplier.size = 7;
        double_multiplier.fracSize = 0;
      }
      new_result = multiply(result, double_multiplier);
      frac_hex -= 16;
      if (result.digits != num->digits)
        free(result.digits);
      result = new_result;
    }
    free(double_multiplier.digits);
    double_multiplier.digits = NULL;

    while (frac_hex > 2) {
      if (!double_multiplier.digits)
        double_multiplier = bignumOfInt(100000000);
      new_result = multiply(result, double_multiplier);
      frac_hex -= 2;
      if (result.digits != num->digits)
        free(result.digits);
      result = new_result;
    }
    free(double_multiplier.digits);
    while (frac_hex) {
      new_result = multiply(result, multiplier);
      frac_hex--;
      if (result.digits != num->digits)
        free(result.digits);
      result = new_result;
    }
    free(multiplier.digits);
    // remove fraction zeros
    shiftRight(&result, result.fracSize);
  }
  char *hex_string = bin_to_hex(result);
  char *dec_string =
      hex_to_dec(hex_string, strlen(hex_string), num->fracSize, isSqrt2);

  if (isSqrt2) {
    printf("1.");
    dec_string[isSqrt2] = '\0';
  }

  printf("%s\n", dec_string);
  free(hex_string);
  free(dec_string);
  if (num->fracSize)
    free(result.digits);
}

void print_bignum_hex(struct bignum *num, long isSqrt2) {
  char *hex_string;
  // shift to left to fit in to hex digits
  size_t shift = 4 - (num->fracSize % 4);
  if (shift < 4) {
    struct bignum result = shiftLeft(*num, shift);
    hex_string = bin_to_hex(result);
    free(result.digits);
  } else {
    shift = 0;
    hex_string = bin_to_hex(*num);
  }
  // remove leading zeros, add dot if needed
  size_t len = strlen(hex_string);
  size_t lead_offset = 0;
  while (hex_string[lead_offset] == '0') {
    lead_offset++;
  }
  if (isSqrt2) {
    if (lead_offset + isSqrt2 < len)
      hex_string[lead_offset + isSqrt2] = '\0';
    printf("0x1.%s\n", hex_string + lead_offset);
    free(hex_string);
    return;
  }
  if (lead_offset + (num->fracSize + shift) / 4 == len) {
    lead_offset--;
    hex_string[0] = '0';
  }
  size_t i = lead_offset;
  for (; i < len; i++) {
    if (i == len - (num->fracSize + shift) / 4) {
      hex_string[i - lead_offset] = '.';
      lead_offset--;
    }
    hex_string[i - lead_offset] = hex_string[i];
  }
  hex_string[i - lead_offset] = '\0';
  printf("0x%s\n", hex_string);
  free(hex_string);
}
