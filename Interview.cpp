#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>
using namespace std;
//
// Modelling a bar diagram:
//
// 1) Define "range string" as a textual representation of a bar diagram: x11:x12:y1, x21:x22:y2, x31:32:y3, ....
//		The range [x11:x12] on X axis has a weight of y1 on the Y axis.
//		The range [x21:x22] on X axis has a weight of y2 on the Y axis and so on.
//
// x) Consider a modelling of bar diagram using the following classes/structures (see code below for definition):
//		SRangeNode	- depicts each range in the bar diagram.
//		CGraphPage	- depicts the entire bar diagram as a linked list of SRangeNodes-s that fits in a page; 
//					the x-axis width of a page (m_iXAxisWidth) if -1, can be considered to have unlimited width, so all
//					ranges can fit in a single page
//		CGraphBook	- depicts a collection of bar diagrams as a dynamic array. Conceptually, a collection of Graph Pages. 
//					  (As long as GraphPage width is unlimited( m_iXAxisWidth is -1), there is only one CGraphPage in a CGraphBook).
//
// x) Your task is to implement the following functions.
//
//		0) CGraphPage::Print() - will print out range string for that CGraphPage on a new line.
//		   CGraphBook::Print() - will print out range string for each CGraphPage in that CGraphBook.
//
//		1) CGraphBook::InsertRange() - will insert new ranges into a CGraphBook. For e.g.,
//			After a InsertRange(86, 97) call, Print() should output "86:97:1"
//			After a InsertRange(101, 160) call, Print() should output "86:97:1, 101:160:1"
//			After a InsertRange(101, 150) call, Print() should output "86:97:1, 101:150:2, 151:160:1" (notice how weight of [101:150] is 2 now).
//
//		2) CGraphBook::DeleteRange() - will delete ranges from a CGraphBook. For e.g. (after the above InsertRange calls),
//			After a DeleteRange(140, 150) call, Print() should output "86:97:1, 101:139:2, 140:160:1"
//			After a DeleteRange(155, 170) call, Print() should output "86:97:1, 101:139:2, 140:154:1"		
//	
//		3) CGraphBook::Repaginate() - will result in more/lesser CGraphPage-s in a CGraphBook by setting member m_iXAxisWidth.
//			Member m_iXAxisWidth depicts limit on the region a CGraphPage can span on X axis.
//			i.e. difference between m_iStart of the first SRangeNode in a CGraphPage and m_iEnd 
//			of the last SRangeNode in that CGraphPage cannot be greater than m_iXAxisWidth of the CGraphBook to which that CGraphPage belongs. 
//			(As already stated, as long as member m_iXAxisWidth is (-1), there is only one CGraphPage in a CGraphBook).
//
//			For e.g.,
//			After a Repaginate(20) call on the CGraphBook call in the above example (after step 2), Print() should output the following.
//			86:97:1, 101:108:2
//			109:128:2
//			129:139:2, 140:148:1
//			149:154:1
//
// x) Please remember that you can't modify the main function
//		Do not change the test inputs; they are specifically designed to test few good scenarios and edge cases
//
// x) Refer to the comments in main function to know what is expected to be printed after each CGraphBook::Print call.
// 
// All the best!
//

class SRangeNode
{
public:
	int m_iStart;
	int m_iEnd;
	int m_iWeight;
	SRangeNode *pNext;

}; 


class CGraphPage
{
private:
	SRangeNode *m_pRanges;

public:
	CGraphPage ()
	{
		m_pRanges = NULL;
	}

	~CGraphPage ()
	{
		delete m_pRanges;
	}

	SRangeNode* Insert(SRangeNode *node)
	{
		//cout << endl << "Start : " << node -> m_iStart << " End : " << node -> m_iEnd << endl;
		SRangeNode *retVal = NULL;
		int inserted = 0;
		if(m_pRanges == NULL)
		{
			m_pRanges = node;
		}
		else
		{
			SRangeNode *tmp = m_pRanges;
			SRangeNode *prev = NULL;
			while(tmp != NULL)
			{
				if(node -> m_iStart < tmp -> m_iStart)
				{
					node -> pNext = tmp;
					if(tmp == m_pRanges)
						m_pRanges = node;
					else
					{
						prev -> pNext = node;
					}
					if(node -> m_iEnd <= tmp -> m_iEnd && node -> m_iEnd >= tmp -> m_iStart)
					{
						retVal = new SRangeNode();
						retVal -> m_iStart = node -> m_iEnd + 1;
						retVal -> m_iEnd = tmp -> m_iEnd;
						retVal -> m_iWeight = 1;
						tmp -> m_iEnd = node -> m_iEnd;
						tmp -> m_iWeight += 1;
						node -> m_iEnd = tmp -> m_iStart - 1;
					}
					else if(node -> m_iEnd > tmp -> m_iEnd)
					{
						tmp -> m_iWeight += 1;
						retVal = new SRangeNode();
						retVal -> m_iStart = tmp -> m_iEnd + 1;
						retVal -> m_iEnd = node -> m_iEnd;
						retVal -> m_iWeight = 1;
						node -> m_iEnd = tmp -> m_iStart - 1;
					}
					inserted = 1;
					break;
				}
				else if(node -> m_iStart >= tmp -> m_iStart  && node -> m_iStart <= tmp -> m_iEnd) 
				{
					if(node -> m_iEnd <= tmp -> m_iEnd)
					{
						//cout << endl << "IN" <<endl;
						node -> pNext = tmp -> pNext;
						tmp -> pNext = node;
						retVal = new SRangeNode();
						retVal -> m_iStart = node -> m_iEnd + 1;
						retVal -> m_iEnd = tmp -> m_iEnd;
						retVal -> m_iWeight = tmp -> m_iWeight;
						node -> m_iWeight = tmp -> m_iWeight + 1;
						tmp -> m_iEnd = node -> m_iStart -1;
					}
					else
					{
						//cout << endl << "IN" <<endl;
						retVal = new SRangeNode();
						if(node -> m_iStart != tmp -> m_iStart)
						{
							node -> pNext = tmp -> pNext;
							tmp -> pNext = node;
							//cout << endl << node -> m_iStart << " " << node -> m_iEnd <<endl;
							//cout << endl << tmp -> m_iStart << " " << tmp -> m_iEnd <<endl;
							node -> m_iWeight = tmp -> m_iWeight + 1;
							retVal -> m_iStart = tmp -> m_iEnd + 1;
							retVal -> m_iEnd = node -> m_iEnd;
							retVal -> m_iWeight = 1;
							tmp -> m_iEnd = node -> m_iStart - 1;
							node -> m_iEnd = tmp -> m_iEnd + 1;
						}
						else
						{
							tmp -> m_iWeight += 1;
							retVal -> m_iStart = tmp -> m_iEnd + 1;
							retVal -> m_iEnd = node -> m_iEnd;
							retVal -> m_iWeight = 1;
						}
						
						//cout << endl << retVal -> m_iStart << " " << retVal -> m_iEnd <<endl;
					}
					inserted = 1;
					break;
				}
				prev = tmp;
				tmp = tmp -> pNext;
			}
			if(inserted != 1)
			{
				if(prev -> m_iEnd + 1 == node -> m_iStart && prev -> m_iWeight == node -> m_iWeight)
				{
					prev -> m_iEnd = node -> m_iEnd;
				}
				else
				{
					prev -> pNext = node;
				}
			}
		}
		return retVal;
	}

	void Delete(int iStart, int iEnd)
	{
		SRangeNode *tmp = m_pRanges;
		SRangeNode *prev = NULL;
		while(tmp != NULL)
		{
			if(iStart >= tmp -> m_iStart && iStart <= tmp -> m_iEnd)
			{
				if(tmp -> m_iStart == iStart)
				{
					if(iEnd > tmp -> m_iEnd)
					{
						if(tmp -> m_iWeight == 1)
						{
							prev -> pNext = tmp -> pNext;
							tmp = prev;
						}
						else
						{
							tmp -> m_iWeight -= 1;
							if(tmp -> pNext -> m_iWeight == tmp -> m_iWeight && tmp -> m_iWeight > 1)
							{
								tmp -> m_iEnd = tmp -> pNext -> m_iEnd;
								tmp -> pNext = tmp -> pNext -> pNext;
							}
							iStart = tmp -> m_iEnd + 1;
						}
					}
					else
					{
						if(iStart == tmp -> m_iStart && iEnd == tmp -> m_iEnd)
						{
							if(tmp -> m_iWeight == 1)
							{
								prev -> pNext = tmp -> pNext;
							}
							else
							{
								tmp -> m_iWeight -= 1;
								if(tmp -> pNext -> m_iWeight == tmp -> m_iWeight && tmp -> m_iWeight > 1)
								{
									tmp -> m_iEnd = tmp -> pNext -> m_iEnd;
									tmp -> pNext = tmp -> pNext -> pNext;
								}
							}
						}
					}
				}
				else if(tmp -> m_iEnd == iEnd)
				{
					tmp -> m_iEnd = iStart - 1;
				}
				else
				{
					int val = tmp -> m_iEnd;
					tmp -> m_iEnd = iStart - 1;
					if(iEnd > tmp -> m_iEnd)
					{
						iStart = val + 1;
					}
				}
			}
			else if(iEnd >= tmp -> m_iStart && iEnd <= tmp -> m_iEnd)
			{
				cout << endl << "weee" << endl;
				if(tmp -> m_iWeight > 1)
				{
					SRangeNode *val = new SRangeNode();
					val -> m_iStart = iEnd + 1;
					val -> m_iEnd = tmp -> m_iEnd;
					val -> m_iWeight = tmp -> m_iWeight;
					//tmp -> m_iEnd = iStart - 1;
					tmp -> m_iWeight -= 1;
					Insert(val);
				}
				else
				{
					tmp -> m_iStart = iEnd + 1;
				}
				/*if(tmp -> m_iWeight > 1)
				{
					SRangeNode *ins = new SRangeNode();
					ins -> m_iStart = 
					tmp -> m_iWeight -= 1;
				}
				tmp -> m_iEnd = iEnd;*/
			}
			else if(iStart < tmp -> m_iStart && iEnd > tmp -> m_iEnd)
			{
				if(tmp -> m_iWeight > 1)
				{
					tmp -> m_iWeight -= 1;
					if(tmp -> pNext -> m_iWeight == tmp -> m_iWeight && tmp -> m_iWeight > 1)
					{
						tmp -> m_iEnd = tmp -> pNext -> m_iEnd;
						tmp -> pNext = tmp -> pNext -> pNext;
					}
				}
				else
				{
					prev -> pNext = tmp -> pNext;
					tmp = prev;
				}
			}
			prev = tmp;
			tmp = tmp -> pNext;
		}
	}
	
	void getUpto(int width)
	{
		SRangeNode *tmp = m_pRanges;
		int length = 0;
		while(tmp != NULL)
		{
			length += (tmp -> m_iEnd - tmp -> m_iStart + 1) * tmp -> m_iWeight;
			if(length > width)
			{
				cout <<endl<< tmp -> m_iStart <<endl;
				SRangeNode *ins = new SRangeNode();
				ins -> m_iStart = (tmp -> m_iEnd - (length - width)) + 1;
				ins -> m_iEnd = tmp -> m_iEnd;
				ins -> m_iWeight = tmp -> m_iWeight;
				tmp -> m_iEnd -= (length - width);
				Insert(ins);
				if(length - width <= width) tmp = tmp -> pNext;
				//length -= width;
				length = 0;
			}
			else if(length == width) length = 0;
			if(tmp != NULL) tmp = tmp -> pNext;
		}
	}
	
	void Print()
	{
		SRangeNode *tmp = m_pRanges;
		while(tmp != NULL)
		{
			cout << tmp -> m_iStart << ":" << tmp -> m_iEnd << ":" << tmp -> m_iWeight << " ";
			tmp = tmp -> pNext;
		}
	}

};

class CGraphBook
{
private:
	CGraphPage *m_pGraphs;
	int m_iXAxisWidth;
public:
	CGraphBook()
	{
		m_pGraphs = new CGraphPage();
		m_iXAxisWidth = -1;
	}

	~CGraphBook ()
	{
	}

	void InsertRange (int iStart, int iEnd)
	{
		SRangeNode *node = new SRangeNode();
		node -> m_iStart = iStart;
		node -> m_iEnd = iEnd;
		node -> m_iWeight = 1;
		node -> pNext = NULL;
		while(node != NULL) 
		{
			node = m_pGraphs -> Insert(node);
			if(node != NULL)
			{
				if(node -> m_iStart > node -> m_iEnd)
				{
					node = NULL;
				}
			}
		}
	}
	
	void DeleteRange (int iStart, int iEnd)
	{
		m_pGraphs -> Delete(iStart, iEnd);
	}

	void Print()
	{
		if(m_iXAxisWidth == -1)
		{
			m_pGraphs -> Print();
		}
	}
	
	void Repaginate(int iXAxisWidth)
	{
		m_pGraphs -> getUpto(iXAxisWidth);
	}


};

int main()
{
	CGraphBook GraphBook;
	int iStart, iEnd;

	// Range 1
	iStart = 57, iEnd = 142;									
	GraphBook.InsertRange (iStart, iEnd);
	printf ("\nInserted [%d:%d] : ", iStart, iEnd);
	GraphBook.Print();	// should print "57:142:1"
	
	// Range 2
	iStart = 91, iEnd = 107;	
	GraphBook.InsertRange (iStart, iEnd);
	printf ("\nInserted [%d:%d] : ", iStart, iEnd);
	GraphBook.Print();	// should print "57:90:1, 91:107:2, 108:142:1"

	// Range 3
	iStart = 107, iEnd = 143;		
	GraphBook.InsertRange (iStart, iEnd);
	printf ("\nInserted [%d:%d]: ", iStart, iEnd);
	GraphBook.Print();	// should print "57:90:1, 91:106:2, 107:107:3, 108:142:2, 143:143:1"
	
	// Range 4
	iStart = 87, iEnd = 90;		
	GraphBook.DeleteRange (iStart, iEnd);
	printf ("\n Deleted [%d:%d]  : ", iStart, iEnd);
	GraphBook.Print();	// should print "57:86:1, 91:106:2, 107:107:3, 108:142:2, 143:143:1"
	
	// Range 5
	iStart = 144, iEnd = 150;			
	GraphBook.InsertRange (iStart, iEnd);
	printf ("\nInserted [%d:%d]: ", iStart, iEnd);
	GraphBook.Print();	// should print "57:86:1, 91:106:2, 107:107:3, 108:142:2, 143:150:1"
		
	// Range 6
	iStart = 91, iEnd = 106;	
	GraphBook.DeleteRange (iStart, iEnd);
	printf ("\n Deleted [%d:%d] : ", iStart, iEnd);
	GraphBook.Print();	// should print "57:86:1, 91:106:1, 107:107:3, 108:142:2, 143:150:1"
	
	// Range 7
	iStart = 20, iEnd = 23;			
	GraphBook.InsertRange (iStart, iEnd);
	printf ("\nInserted [%d:%d]  : ", iStart, iEnd);
	GraphBook.Print();	// should print "20:23:1, 57:86:1, 91:106:1, 107:107:3, 108:142:2, 143:150:1"
	
	// Range 8
	iStart = 45, iEnd = 65;			
	GraphBook.InsertRange (iStart, iEnd);
	printf ("\nInserted [%d:%d]  : ", iStart, iEnd);
	GraphBook.Print();	// should print "20:23:1, 45:56:1, 57:65:2, 66:86:1, 91:106:1, 107:107:3, 108:142:2, 143:150:1"

	// Range 9
	iStart = 107, iEnd = 107;
	GraphBook.DeleteRange (iStart, iEnd);
	printf ("\n Deleted [%d:%d]: ", iStart, iEnd);
	GraphBook.Print();	// should print "20:23:1, 45:56:1, 57:65:2, 66:86:1, 91:106:1, 107:142:2, 143:150:1"

	// Range 10
	iStart = 22, iEnd = 140;
	GraphBook.DeleteRange (iStart, iEnd);
	printf ("\n Deleted [%d:%d] : ", iStart, iEnd);
	GraphBook.Print();	// should print "20:21:1, 57:86:1, 107:140:1, 141:142:2, 143:150:1"

						// Correct Ans 20:21:1, 57:65:1, 107:140:1, 141:142:2, 143:150:1


	int iNewPageSize = 40;
	GraphBook.Repaginate (iNewPageSize);
	printf ("\nRepaginated to [%d]:", iNewPageSize);
	GraphBook.Print();	// should print
						// 20:21:1, 57:84:1
						// 85:86:1, 107:140:1, 141:142:2, 143:144:1
						// 145:150:1

	printf ("\n");
	return 0;
}
