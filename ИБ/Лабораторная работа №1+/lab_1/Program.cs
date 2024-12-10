using System;
using System.IO;

namespace lab_1
{
    class Program
    {
        static void Encryption(char[] textIn,int key)//Зашифровка
        {
            char[] ABC = new char[] { 'А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ё', 'Ж', 'З', 'И', 'Й', 'К', 'Л', 'М', 'Н', 'О', 'П', 'Р', 'С', 'Т', 'У', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 'Щ', 'Ъ', 'Ы', 'Ь', 'Э', 'Ю', 'Я' };
            char[] EncryptABC = new char[] { 'А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ё', 'Ж', 'З', 'И', 'Й', 'К', 'Л', 'М', 'Н', 'О', 'П', 'Р', 'С', 'Т', 'У', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 'Щ', 'Ъ', 'Ы', 'Ь', 'Э', 'Ю', 'Я' };
            //Преобразование алфавита
            if (key < 0)
            {
                for (int i = Math.Abs(key); i < ABC.Length; i++)
                {
                    EncryptABC[i] = ABC[i + key];
                }
                for (int i = 0; i < Math.Abs(key); i++)
                {
                    EncryptABC[i] = ABC[ABC.Length + key + i];
                }
            }
            else
            {
                for (int i = 0; i < ABC.Length-key; i++)
                {
                    EncryptABC[i] = ABC[i + key];
                }
                for (int i = 0; i < key; i++)
                {
                    EncryptABC[ABC.Length - key+i] = ABC[i];
                }
            }

            //Расшифровка
            for (int i = 0; i < textIn.Length-1000; i++)
            {
                for (int j = 0; j < ABC.Length ; j++)
                {
                    if (textIn[i] == ABC[j])
                    {
                        textIn[i] = EncryptABC[j];
                        break;
                    }
                    if (textIn[i].ToString() == ABC[j].ToString().ToLower())
                    {
                        textIn[i] = Convert.ToChar(EncryptABC[j].ToString().ToLower());
                        break;
                    }
                }
            }
            Console.WriteLine("Готово!");
            for (int i = 0; i < textIn.Length - 1000; i++)
            {
                Console.Write(textIn[i]);
            }
            Console.WriteLine();
            //Запись ответа
            FileStream fileOut = new FileStream("OUT.txt", FileMode.Open);
            StreamWriter writer = new StreamWriter(fileOut);
            for (int i = 0; i < textIn.Length - 1000; i++)
            {
                writer.Write(textIn[i]);
            }
            writer.Close();
        }

        static void Decryption(char[] textIn, int key)//Расшифровка
        {
            char[] ABC = new char[] { 'А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ё', 'Ж', 'З', 'И', 'Й', 'К', 'Л', 'М', 'Н', 'О', 'П', 'Р', 'С', 'Т', 'У', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 'Щ', 'Ъ', 'Ы', 'Ь', 'Э', 'Ю', 'Я' };
            char[] EncryptABC = new char[] { 'А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ё', 'Ж', 'З', 'И', 'Й', 'К', 'Л', 'М', 'Н', 'О', 'П', 'Р', 'С', 'Т', 'У', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 'Щ', 'Ъ', 'Ы', 'Ь', 'Э', 'Ю', 'Я' };
            //Преобразование алфавита
            if (key < 0)
            {
                for (int i = Math.Abs(key); i < ABC.Length; i++)
                {
                    EncryptABC[i] = ABC[i + key];
                }
                for (int i = 0; i < Math.Abs(key); i++)
                {
                    EncryptABC[i] = ABC[ABC.Length + key + i];
                }
            }
            else
            {
                for (int i = 0; i < ABC.Length - key; i++)
                {
                    EncryptABC[i] = ABC[i + key];
                }
                for (int i = 0; i < key; i++)
                {
                    EncryptABC[ABC.Length - key + i] = ABC[i];
                }
            }

            //Расшифровка
            for (int i = 0; i < textIn.Length - 1000; i++)
            {
                for (int j = 0; j < ABC.Length; j++)
                {
                    if (textIn[i] == EncryptABC[j])
                    {
                        textIn[i] = ABC[j];
                        break;
                    }
                    if (textIn[i].ToString() == EncryptABC[j].ToString().ToLower())
                    {
                        textIn[i] = Convert.ToChar(ABC[j].ToString().ToLower());
                        break;
                    }
                }
            }
            Console.WriteLine("Готово!");
            for (int i = 0; i < textIn.Length - 1000; i++)
            {
                Console.Write(textIn[i]);
            }
            Console.WriteLine();

            //Запись ответа
            FileStream fileOut = new FileStream("OUT.txt", FileMode.Open);
            StreamWriter writer = new StreamWriter(fileOut);
            for (int i = 0; i < textIn.Length - 1000; i++)
            {
                writer.Write(textIn[i]);
            }
            writer.Close();
        }

        static void Main(string[] args)
        {
            int namber=0;
            while (namber != 3)
            {
                Console.WriteLine("Выберите действие:");
                Console.WriteLine("1) Зашифровать");
                Console.WriteLine("2) Расшифровать");
                Console.WriteLine("3) Выход");
                namber = Convert.ToInt32(Console.ReadLine());
                switch (namber)
                {
                    case 1:
                        Console.WriteLine("Зашифровка");
                        FileStream fileIn1 = new FileStream("IN.txt", FileMode.Open);
                        StreamReader reader1 = new StreamReader(fileIn1);
                        char[] textIn = new char[reader1.Peek()];
                        Console.Write("Исходный текст: ");
                        for (int i=0; reader1.Peek() >= 0; i++)
                        {
                            textIn[i] = Convert.ToChar(reader1.Read());
                            Console.Write(textIn[i]);
                        }
                        Console.WriteLine();
                        reader1.Close();

                        int key;
                        Console.Write("Введите ключ шифрования: ");
                        key = Convert.ToInt32(Console.ReadLine());
                        Encryption(textIn, key);
                        break;

                    case 2:
                        Console.WriteLine("Расшифровка");
                        FileStream fileIn2 = new FileStream("IN.txt", FileMode.Open);
                        StreamReader reader2 = new StreamReader(fileIn2);
                        char[] textIn2 = new char[reader2.Peek()];
                        Console.Write("Исходный текст: ");
                        for (int i = 0; reader2.Peek() >= 0; i++)
                        {
                            textIn2[i] = Convert.ToChar(reader2.Read());
                            Console.Write(textIn2[i]);
                        }
                        Console.WriteLine();
                        reader2.Close();

                        Console.Write("Введите ключ шифрования: ");
                        key = Convert.ToInt32(Console.ReadLine());
                        Decryption(textIn2, key);
                        break;
                }
            }
        }
    }
}
