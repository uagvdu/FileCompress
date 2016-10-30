#pragma once 
#include"HuffmanTree.h"
#include<cstring>
/*step1 :统计字符出现的次数
step2：根据次数作为叶子节点，创建huffman树
step3: 利用huffman树得到对应的编码，
step4 ：压缩
*/

/*step1 :创建huffman树
step2：按位读取，还原字符   
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

class FileCompress
{
public:

	string Compress(char* filename)
	{
		FILE* fout = fopen(filename, "r");
		assert(fout);
		char ch = getc(fout);
		while (ch != EOF)
		{
			a[ch]._ch = ch;
			a[ch]._count ++;
			ch = getc(fout);
		}

		charInfo invalue;
		HuffmanTree<charInfo>  huf(a,256,invalue);   //_data为charInfo类型,空格也是字符，     step1 and 2 over
		HuffmanNode<charInfo>* root = huf.Getroot();
 		
		Getcode(root);//获取code
		
		//step4 压缩的时候是8位8位的压缩
		string CFileName = filename;
		CFileName += ".huf";
		FILE *fin = fopen(CFileName.c_str(), "w");
		assert(fin);
		int pos = 0;
		char value = 0;  //字符类型也是一字节char类型，然后想知道该字符的二进制，则只需知道该字符对应的ascll为多少。
		                //所以在文件里面读取的是一字节int类型的二进制，而非‘01000110’等（最开始进入误区然后自己纠结了半天）。
		fseek(fout, SEEK_SET, 0);//从首位置不偏移开始读取数据
		ch = fgetc(fout);
		while (ch!=EOF)
		{
			string code = a[ch]._code;
			for (int i = 0; i < code.size(); i++)
			{
				value <<= 1;   //先移位再或，否则的话，满8位之后又会移位一次，造成数据不匹配
				value |= code[i]-48;//要得到的是0 ， 1编码，value 是char类型，但他也是一字节int类型
				
				pos++;

				if (pos == 8)
				{
					putc(value, fin);
					pos = 0;
					value = 0;
				}
			}
			
			ch = getc(fout);
		}
		if (pos < 8)     //文件中的数据大小至少要为1字节
		{
			value <<= 8 - pos;
			fputc(value,fin);
		}

		string configname = filename;
		int index = configname.find('.');
		configname = configname.substr(0,index);
		configname += ".config";
		FILE* foutconfig = fopen(configname.c_str(),"w");
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

	string UncompressFile(char *filename)
	{
	 

		FILE* fout = fopen(filename,"r");    //压缩文件
		assert(fout);

		string UCFilename = filename;
		size_t index = UCFilename.find('.');
		UCFilename = UCFilename.substr(0, index);

		string configname = filename;
		configname = configname.substr(0,index);

		configname += ".config";
		FILE* foutconfig = fopen(configname.c_str(),"r");          //配置文件
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
		FILE* fin = fopen(UCFilename.c_str(), "w");             //解压缩文件
		assert(fin);

		charInfo invalue;
		HuffmanTree<charInfo> hf(a,256,invalue);
		HuffmanNode<charInfo>* root = hf.Getroot();
		
		Long charcount = root->_data._count;//charcount 代表所有字符出现的次数，当其为零，当然也代表了字符解压缩完毕
		int pos = 7;
		char ch = getc(fout);   
		HuffmanNode<charInfo>* cur = root;
		while (ch != EOF)   //按位读取，还原字符  :按位读取 .huf文件里面的code，对比huffman的节点，当节点为叶子的时候，还原所对应的字符
		{
			
			while (cur)
			{
				//Long& charcount = cur->_data._count;            //虽然能解决末尾出现多余字符的问题，但此处用引用不好的原因是将huffman数据里面的_count给改变了，破坏了huffman树的完整性
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
					if (ch&(1 << pos)) //if (ch&(1 << pos)==1) 所以类似于压缩代码中的value原因，当他&之后，得到的是二进制数据，但它转换为char类型的时候，值不一定会是1。
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

	void Getcode(HuffmanNode<charInfo> *& root)  //step 3 利用huffman树得到code ，仅仅是每个字符的编码
	{
		if (root == NULL)
			return;
		else
		{
			HuffmanNode<charInfo> *cur = root;
			HuffmanNode<charInfo> *leaf = root;
			Getcode(cur->_left);  
			Getcode(cur->_right);   //只有当为叶子节点的时候才是找到该字符的_code的开始

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
	//fc.Compress("test.txt");
	fc.UncompressFile("test.txt.huf");

}