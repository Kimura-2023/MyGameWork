using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BreakImage : MonoBehaviour
{
    static public BreakImage instance;
    public byte[] image;        // クリア時演出用画像保持用変数
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
