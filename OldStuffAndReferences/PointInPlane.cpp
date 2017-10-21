 
bool isPointInRect(Vector2 point, StormVertex* vertices) {
    Vector2 P1 = vertices[0].position;
    Vector2 P2 = vertices[1].position;
    Vector2 P3 = vertices[2].position;
    Vector2 P4 = vertices[3].position;
    Vector2 P = point;

    Vector2 P1_P4 = P1 - P4;
    Vector2 P3_P4 = P3 - P4;
    Vector2 TWO_P_C = (P * 2.0f) - P1 - P3;    // TWO_P_C=2P-C, C=Center of rectangle
   
    return (P3_P4.dot(TWO_P_C - P3_P4) <= 0 && P3_P4.dot(TWO_P_C + P3_P4) >= 0) &&
           (P1_P4.dot(TWO_P_C - P1_P4) <= 0 && P1_P4.dot(TWO_P_C + P1_P4) >= 0);
}

sol::default_constructor