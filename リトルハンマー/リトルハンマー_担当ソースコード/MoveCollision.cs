using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UniRx;

public class MoveCollision : MonoBehaviour
{
    public ReactiveProperty<bool> MoveStop = new ReactiveProperty<bool>();
    public ReactiveProperty<bool> Move = new ReactiveProperty<bool>();
    [SerializeField] private ParticleSystem Hitparticle;
    private float distance;
    GameObject Block;
    ParticleSystem newparticle;

    private void Start()
    {
        newparticle = Instantiate(Hitparticle);
        Block = this.transform.parent.gameObject;
        distance = 1.0f;
    }

    // 動くブロックの横当たり判定に持たせる
    private void OnTriggerEnter(Collider collider)
    {
        Vector3 hitPos = collider.ClosestPointOnBounds(this.transform.position);
        hitPos.z = -this.transform.parent.localScale.z / 2;
        newparticle.transform.position = hitPos;
        newparticle.transform.localScale = transform.parent.localScale;

        if (collider.tag != "hammer" && Block.GetComponent<Block>().blockStatus.IsImpact)
        {
            newparticle.Play();
            Debug.Log("ああ");
        }
    }

    //private void OnTriggerStay(Collider other)
    //{
    //    if (other.tag != "hammer")
    //    {
    //        Debug.Log("ああ");
    //        MoveStop.Value = true;
    //        Move.Value = false;
    //    }

    //}

    //private void OnTriggerExit(Collider collider)
    //{
    //    if (collider.tag != "hammer")
    //    {
    //        MoveStop.Value = false;
    //        Move.Value = true;
    //    }
    //}

    private void FixedUpdate()
    {
        if(CheckCollision())
        {
            MoveStop.Value = true;
            Move.Value = false;
        }
        else
        {
            MoveStop.Value = false;
            Move.Value = true;
        }
    }

    // 当たり判定(レイキャスト
    private bool CheckCollision()
    {
        bool isStop = false;
        RaycastHit hit;
        Vector3 rayHPosition = transform.parent.position + new Vector3(0.0f, transform.parent.localScale.y / 2 - 0.5f, 0.0f); // 高い方
        Vector3 rayDPosition = transform.parent.position + new Vector3(0.0f, -transform.parent.localScale.y / 2 + 0.5f, 0.0f);// 低い方
        Ray Hray = new Ray(rayHPosition, Vector3.right * Block.GetComponent<Block>().dir);
        Ray Dray = new Ray(rayDPosition, Vector3.right * Block.GetComponent<Block>().dir);
        Debug.DrawRay(rayHPosition, Vector3.right * distance * transform.parent.localScale.x/2 * Block.GetComponent<Block>().dir, Color.red);
        Debug.DrawRay(rayDPosition, Vector3.right * distance * transform.parent.localScale.x / 2 * Block.GetComponent<Block>().dir, Color.red);

        if (Physics.Raycast(Hray, out hit, distance * transform.parent.localScale.x/2) || Physics.Raycast(Dray, out hit, distance * transform.parent.localScale.x / 2))
        {
            if (hit.collider.tag != "hammer")
            {
                //Debug.Log("Hit");
                isStop = true;
            }
        }
        return isStop;
    }
}
