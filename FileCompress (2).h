#pragma once 
#include"HuffmanTree.h"
#include<cstring>
/*step1 :ͳ���ַ����ֵĴ���
step2�����ݴ�����ΪҶ�ӽڵ㣬����huffman��
step3: ����huffman���õ���Ӧ�ı��룬
step4 ��ѹ��
*/

/*step1 :����huffman��
step2����λ��ȡ����ԭ�ַ�   
*/
typedef long long Long;


struct charInfo
{
	Long _count;
	string _code;
	unsigned char _ch;
	charInfo()
		:_count(0)
	{}

	bool operator!= (charInfo b)
	{
		return _count != b._count ? 1: 0;
	}

	charInfo operator+ (charInfo b)
	{
		charInfo tmp;
		tmp._count = _count + b._count;
		return tmp;
	}

	bool operator< (charInfo b)
	{
		return _count < b._count ? 1 : 0;
	}
};

class FileCompress  /*ѹ���ļ��У�ֻ��ѹ���ļ���������ļ���˼·��1. �½�һ���ļ��У�2. ��Դ�ļ���������ļ�ѹ�����õ������ļ���ѹ���ļ�
	3.���½����ļ������洴���µ�ͬ���ļ��У�4.ֱ��ת�������ļ���ѹ���ļ����µ��ļ����е��ļ�����*/
{
public:

	string Compress( char* filename)
	{

		FILE* fout = fopen(filename, "rb"); 
		assert(fout);
		unsigned char ch = getc(fout);
		while (!feof(fout))
		{
			a[ch]._ch = ch;
			a[ch]._count ++;
			ch = getc(fout);
		}

		charInfo invalue;
		HuffmanTree<charInfo>  huf(a,256,invalue);   //_dataΪcharInfo����,�ո�Ҳ���ַ���     step1 and 2 over
		HuffmanNode<charInfo>* root = huf.Getroot();
 		
		Getcode(root);//��ȡcode
		
		//step4 ѹ����ʱ����8λ8λ��ѹ��
		string CFileName = filename;
		CFileName += ".huf";
		FILE *fin = fopen(CFileName.c_str(), "wb"); 
		assert(fin);
		int pos = 0;
		char value = 0;  //�ַ�����Ҳ��һ�ֽ�char���ͣ�Ȼ����֪�����ַ��Ķ����ƣ���ֻ��֪�����ַ���Ӧ��ascllΪ���١�
		                //�������ļ������ȡ����һ�ֽ�int���͵Ķ����ƣ����ǡ�01000110���ȣ��ʼ��������Ȼ���Լ������˰��죩��
		                //����λ�ò�ƫ�ƿ�ʼ��ȡ����
		
       
		fseek(fout, SEEK_SET ,0 );
	     ch = fgetc(fout);

		while (!feof(fout))
		{
			string code = a[ch]._code;
			for (int i = 0; i < code.size(); i++)
			{
				value <<= 1;   //����λ�ٻ򣬷���Ļ�����8λ֮���ֻ���λһ�Σ�������ݲ�ƥ��
				value |= code[i]-48;//Ҫ�õ�����0 �� 1���룬value ��char���ͣ�����Ҳ��һ�ֽ�int����
				
				pos++;

				if (pos == 8)
				{
					putc(value, fin);
					pos = 0;
					value = 0;
				}
			}
			
			ch = fgetc(fout);
		}
		if (pos < 8)     //�ļ��е����ݴ�С����ҪΪ1�ֽ�
		{
			value <<= 8 - pos;
			fputc(value,fin);
		}

		string configname = filename;
		int index = configname.find('.');
		configname = configname.substr(0,index);
		configname += ".config";
		FILE* foutconfig = fopen(configname.c_str(),"wb");
		string line;
		char s_count[100];
		for (int i = 0; i < 256;i++)
		{
			if (a[i]._count != 0)
			{
				line += a[i]._ch;
				line += ',';
				_itoa(a[i]._count, s_count,10);
				line += s_count;
				line += '\n';
				fputs(line.c_str(), foutconfig);
				line.clear();
			}

		}
		
		fclose(foutconfig);
		fclose(fout);
		fclose(fin);
		return CFileName;

	}

	string UncompressFile(const char *filename)
	{
	 

		FILE* fout = fopen(filename,"rb");    //ѹ���ļ�
		assert(fout);

		string UCFilename = filename;
		size_t index = UCFilename.find('.');
		UCFilename = UCFilename.substr(0, index);

		string configname = filename;
		configname = configname.substr(0,index);

		configname += ".config";
		FILE* foutconfig = fopen(configname.c_str(),"rb");          //�����ļ�
		assert(foutconfig);

		string line;	
		Long count = 0;
		while (Readline(foutconfig, line))
		{
			if (line.empty())
			{
				continue;
			}
			unsigned  char ch = line[0];
			count = atoi(line.substr(2, line.size()).c_str());
			a[ch]._count = count;
			a[ch]._ch = ch;
			line.clear();
			
			
		}



		UCFilename += ".unhuf";
		FILE* fin = fopen(UCFilename.c_str(), "wb");             //��ѹ���ļ�
		assert(fin);

		charInfo invalue;
		HuffmanTree<charInfo> hf(a,256,invalue);
		HuffmanNode<charInfo>* root = hf.Getroot();
		
		Long charcount = root->_data._count;//charcount ���������ַ����ֵĴ���������Ϊ�㣬��ȻҲ�������ַ���ѹ�����
		int pos = 7;
		unsigned char ch = getc(fout);   
		HuffmanNode<charInfo>* cur = root;
		while (!feof(fout))   //��λ��ȡ����ԭ�ַ�  :��λ��ȡ .huf�ļ������code���Ա�huffman�Ľڵ㣬���ڵ�ΪҶ�ӵ�ʱ�򣬻�ԭ����Ӧ���ַ�
		{
			
			while (cur)
			{
				//Long& charcount = cur->_data._count;            //��Ȼ�ܽ��ĩβ���ֶ����ַ������⣬���˴������ò��õ�ԭ���ǽ�huffman���������_count���ı��ˣ��ƻ���huffman����������
				if (cur->_left == NULL&&cur->_right == NULL)
				{
					if (charcount != 0)
					{
						fputc(cur->_data._ch, fin);
						charcount--;
						cur = root;
						
					}
					else
						break;
				}
				else
				{
					if (ch&(1 << pos)) //if (ch&(1 << pos)==1) ����������ѹ�������е�valueԭ�򣬵���&֮�󣬵õ����Ƕ��������ݣ�����ת��Ϊchar���͵�ʱ��ֵ��һ������1��
					{
						cur = cur->_right;
					}
					else
					{
						cur = cur->_left;
					}
					pos--;

					if (pos < 0)
					{
						pos = 7;
						ch = getc(fout);
					}
				}
			}
			break;

		}
		fclose(fin);
		fclose(fout);
		return UCFilename;
	}

protected:    
	bool Readline(FILE* fout,string& line)
	{
		assert(fout);
		char ch = fgetc(fout);
		if (ch == EOF)
			return false;
		line += ch;
		ch = getc(fout);
		while ((ch !=EOF)&&(ch!='\n'))
		{
			line += ch;
			ch = fgetc(fout);
		}
		
		return true;
	}

	void Getcode(HuffmanNode<charInfo> *& root)  //step 3 ����huffman���õ�code ��������ÿ���ַ��ı���
	{
		if (root == NULL)
			return;
		else
		{
			HuffmanNode<charInfo> *cur = root;
			HuffmanNode<charInfo> *leaf = root;
			Getcode(cur->_left);  
			Getcode(cur->_right);   //ֻ�е�ΪҶ�ӽڵ��ʱ������ҵ����ַ���_code�Ŀ�ʼ

			if (cur->_left == NULL&&cur->_right == NULL)
			{
				while (cur->_parent)
				{
					HuffmanNode<charInfo> *parent = cur->_parent;

					if (parent->_left == cur)
					{
						leaf->_data._code += '0';
						a[leaf->_data._ch]._code += '0';
					}
					else
					{
						leaf->_data._code += '1';
						a[leaf->_data._ch]._code += '1';

					}
					cur = cur->_parent;
				}
				reverse(leaf->_data._code.begin(), leaf->_data._code.end());
				reverse(a[leaf->_data._ch]._code.begin(), a[leaf->_data._ch]._code.end());

			}
			
		}
	}

	
protected:
	charInfo a[256];
}; 

void CompressFileTest()
{
	FileCompress fc;
  

	fc.Compress("D:\\vs\\program\\�ļ�ѹ��\\�ļ�ѹ��\\Test\\�����Ƥ��� .mp3");
	fc.UncompressFile("D:\\vs\\program\\�ļ�ѹ��\\�ļ�ѹ��\\Test\\�����Ƥ��� .mp3.huf");
	 /* fc.Compress("D:\\vs\\program\\�ļ�ѹ��\\�ļ�ѹ��\\Test\\81.jpg");
	fc.UncompressFile("D:\\vs\\program\\�ļ�ѹ��\\�ļ�ѹ��\\Test\\81.jpg.huf");
	fc.Compress("D:\\vs\\program\\�ļ�ѹ��\\�ļ�ѹ��\\Test\\���̽��C++����ģ��.pdf");
	fc.UncompressFile("D:\\vs\\program\\�ļ�ѹ��\\�ļ�ѹ��\\Test\\���̽��C++����ģ��.pdf.huf");*/

}

void CompressFileTest2()
{
	FileCompress fc;
	fc.UncompressFile("81.jpg.huf");

}
void CompressFileTest3()
{
	FileCompress fc;
	 
	fc.UncompressFile("hh.mp3.huf");

}
void CompressFileTest4()
{
	FileCompress fc;
  
	fc.UncompressFile("���̽��C++����ģ��.pdf.huf");

}