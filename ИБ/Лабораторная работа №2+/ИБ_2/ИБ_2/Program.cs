using System;
using System.IO;
using System.Text;

namespace ИБ_2
{
    class Program
    {
        const uint Delta = 0x9E3779B9;
        public static uint[] Encryption(uint[] text, uint[] key)
        {
            uint t0 = text[0], t1 = text[1];
            uint k0 = key[0], k1 = key[1], k2 = key[2], k3 = key[3];
            uint sum = 0;
            for (int i = 0; i < 32; i++)
            {
                sum += Delta;
                t0 += ((t1 << 4) + k0) ^ (t1 + sum) ^ ((t1 >> 5) + k1);
                t1 += ((t0 << 4) + k2) ^ (t0 + sum) ^ ((t0 >> 5) + k3);
            }
            return new uint[] { t0, t1 };
        }
        public static uint[] Decryption(uint[] text, uint[] key)
        {
            uint t0 = text[0], t1 = text[1];
            uint k0 = key[0], k1 = key[1], k2 = key[2], k3 = key[3];
            ulong sumL = (ulong)Delta * 32;

            uint sum = (uint)sumL;
            for (int i = 0; i < 32; i++)
            {
                t1 -= ((t0 << 4) + k2) ^ (t0 + sum) ^ ((t0 >> 5) + k3);
                t0 -= ((t1 << 4) + k0) ^ (t1 + sum) ^ ((t1 >> 5) + k1);
                sum -= Delta;
            }
            return new uint[] { t0, t1 };
        }

        static void Main(string[] args)
        {
            //приём сообщения
            byte[] bytesMessage;
            using (FileStream MessageFile = new FileStream("IN.txt", FileMode.Open))
            using (BinaryReader reader = new BinaryReader(MessageFile))
            {
                //Console.WriteLine(reader.CurrentEncoding.WebName); //вывод формата входного текста
                //string Message = reader.ReadToEnd();
                //bytesMessage = Encoding.UTF8.GetBytes(Message);
                bytesMessage = reader.ReadBytes(reader.PeekChar());
            }
            if (bytesMessage.Length % 8 != 0)
            {
                Array.Resize(ref bytesMessage, (bytesMessage.Length / 8 + 1) * 8);
            }
            //приём сообщения

            //приём ключа
            uint[] key = new uint[4];
            using (FileStream KeyFile = new FileStream("KEY.txt", FileMode.Open))
            using (StreamReader reader = new StreamReader(KeyFile))
            {
                string keyString = reader.ReadToEnd();
                for (int i = 0; i < 4; i++)
                {
                    key[i] = Convert.ToUInt32(keyString.Substring(i * 8, 8), 16);
                }
            }
            //приём ключа

            //шифровка и расшифровка сообщения
            using (FileStream EncryptionFile = new FileStream("ENCRYPTION.txt", FileMode.Create))
            using (StreamWriter encryptedWriter = new StreamWriter(EncryptionFile))
            using (FileStream DecryptionFile = new FileStream("OUT.txt", FileMode.Create))
            using (BinaryWriter decryptedWriter = new BinaryWriter(DecryptionFile))
            {
                for (int i = 0; i < bytesMessage.Length; i += 8)
                {
                    //Разбиение
                    if (i + 4 >= bytesMessage.Length) break; // Защита от выхода за границы
                    uint[] t = new uint[2];
                    t[0] = BitConverter.ToUInt32(bytesMessage, i);
                    t[1] = BitConverter.ToUInt32(bytesMessage, i + 4);

                    // Шифрование
                    uint[] encrypted = Encryption(t, key);
                    encryptedWriter.Write("{0:X}{1:X}", encrypted[0], encrypted[1]);

                    // Расшифровка
                    uint[] decrypted = Decryption(encrypted, key);
                    byte[] decryptedBytes = new byte[8];
                    BitConverter.GetBytes(decrypted[0]).CopyTo(decryptedBytes, 0);
                    BitConverter.GetBytes(decrypted[1]).CopyTo(decryptedBytes, 4);
                    decryptedWriter.Write(decryptedBytes);
                }
            }
            //шифровка и расшифровка сообщения

            Console.WriteLine("Шифрование и расшифровка завершены. Результаты записаны в файлы.");
            Console.ReadKey();
            Console.ReadKey();
            Console.ReadKey();
            Console.ReadKey();

        }
    }
}