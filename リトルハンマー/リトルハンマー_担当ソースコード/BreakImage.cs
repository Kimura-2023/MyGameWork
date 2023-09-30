using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BreakImage : MonoBehaviour
{
    static public BreakImage instance;
    public byte[] image;        // �N���A�����o�p�摜�ێ��p�ϐ�
    // Start is called before the first frame update
    void Start()
    {
        this.gameObject.AddComponent<DontDestroy>();
        if (instance == null)
        {
            instance = this;
        }
        else
        {
            Destroy(this.gameObject);
        }
    }

    public void SetImage(byte[] by)
    {
        image = by;
    }
    public byte[] GetImage()
    {
        return image;
    }
}
