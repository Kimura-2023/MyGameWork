using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]public class BlockStatus
{
    // Start is called before the first frame update
    private bool isImpact = false;
    private bool onWallFace = false;
    

    public bool IsImpact
    {
        get;
        set;
    }

    public bool OnWallFace
    {
        get;
        set;
    }

}
