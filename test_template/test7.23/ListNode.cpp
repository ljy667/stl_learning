//给你两个单链表的头节点headA和headB,请你找出并返回两个单链表相交的起始节点，
//如果两个链表不存在相交节点，返回null
#include <iostream>


class Solution_ListNode{

};

struct ListNode{
    int val;
    ListNode* next;
    ListNode(int x):val(x),next(nullptr){}
};

int main()
{
    ListNode* head =new ListNode(1);
    ListNode* second =new ListNode(2);
    ListNode* third =new ListNode(3);
    ListNode* fourth =new ListNode(4);

    head->next=second;
    second->next=third;
    third->next=fourth;

    std::cout<<"val";
    ListNode* current=head;
    while(current!=nullptr)
    {
      std::cout<<current->val<<"" ;
      current=current->next;
    }
    std::cout<<std::endl;
     
    ListNode* newNoed =new ListNode(5);
    newNoed->next = second->next;
    second ->next =newNoed;

    std::cout << "after";
    current = head;
    while(current!=nullptr)
    {
      std::cout << current->val << "";  
      current = current->next;
    }
    std::cout << std::endl;

     // 删除节点（例如删除第三个节点）
    // 找到前驱节点
    ListNode* prev = head;
    while (prev->next != nullptr && prev->next != third) {
        prev = prev->next;
    }
    // 删除第三个节点
    if (prev->next == third) {
        prev->next = third->next;
        delete third;
    }
 
    // 遍历链表，验证删除
    std::cout << "删除后链表：";
    current = head;
    while (current != nullptr) {
        std::cout << current->val << " ";
        current = current->next;
    }
    std::cout << std::endl;

    // 释放所有节点的内存
    current = head;
    while (current != nullptr) {
        ListNode* temp = current;
        current = current->next;
        delete temp;
    }

    return 0;
  
}